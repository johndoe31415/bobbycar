#	defiant - Modded Bobby Car toy for toddlers
#	Copyright (C) 2020-2020 Johannes Bauer
#
#	This file is part of defiant.
#
#	defiant is free software; you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation; this program is ONLY licensed under
#	version 3 of the License, later versions are explicitly excluded.
#
#	defiant is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#
#	You should have received a copy of the GNU General Public License
#	along with defiant; if not, write to the Free Software
#	Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
#
#	Johannes Bauer <JohannesBauer@gmx.de>

import os
import subprocess
import glob
import hashlib
import struct
import json
import contextlib

class BaseCommand():
	def __init__(self, cmdname, args):
		self._cmdname = cmdname
		self._args = args
		self.run()

class ExtractAudioCommand(BaseCommand):
	def run(self):
		subprocess.check_call([ "ffmpeg", "-i", self._args.input_file, "-vn", "-ar", "44100", "-ac", "2", "-f", "wav", self._args.output_file ])

class CompileAudioImageCommand(BaseCommand):
	def _hashfile(self, filename):
		hashval = hashlib.md5()
		with open(filename, "rb") as f:
			while True:
				chunk = f.read(1024 * 1024)
				if len(chunk) == 0:
					break
				hashval.update(chunk)
		return hashval.hexdigest()

	def _generate_toc(self):
		toc = {
			"_comment":		"For Emmy <3 by Dad. https://github.com/johndoe31415/defiant",
			"entries":		[ ],
		}
		for filename in self._file_names:
			entry = {
				"name":			filename,
				"filesize":		os.stat(filename).st_size,
				"src_md5":		self._hashfile(filename),
			}
			toc["entries"].append(entry)
		return toc

	def _rawify(self, input_filename):
		return subprocess.check_output([ "sox", input_filename, "-r", "11025", "-e", "unsigned", "-b", "8", "-c", "1", "-t", "raw", "-" ])

	def _pad_to(self, data, length):
		assert(len(data) <= length)
		if length > len(data):
			data += bytes([ 0xff ]) * (length - len(data))
		return data

	def _generate_content(self, base_offset, alignment):
		assert((base_offset % alignment) == 0)
		content = [ ]
		offset = base_offset
		for filename in self._file_names:
			data = self._rawify(filename)
			size = len(data)
			padded_data_size = (len(data) + alignment - 1) // alignment * alignment
			data = self._pad_to(data, padded_data_size)
			entry = {
				"offset":	offset,
				"name":		os.path.splitext(os.path.basename(filename))[0],
				"data":		data,
				"size":		size,
			}
			content.append(entry)
			offset += padded_data_size
		return content

	def _generate_image(self):
		image = bytearray()

		# Binary TOC first
		binary_toc = bytearray()
		for entry in self._content:
			binary_entry = struct.pack("< L L 56s", entry["offset"], entry["size"], entry["name"].encode())
			binary_toc += binary_entry
		binary_toc = self._pad_to(binary_toc, 4096)
		image += binary_toc

		# JSON TOC then
		json_toc = self._pad_to(json.dumps(self._toc, sort_keys = True).encode("ascii"), 4096)
		image += json_toc

		for entry in self._content:
			image += entry["data"]

		return image

	def run(self):
		self._file_names = sorted(glob.glob(self._args.input_dir + "/*.wav"))
		self._toc = self._generate_toc()
		self._content = self._generate_content(base_offset = 8192, alignment = 4096)
		self._image = self._generate_image()
		with open(self._args.output_file, "wb") as f:
			f.write(self._image)

class DecompileAudioImageCommand(BaseCommand):
	def run(self):
		with open(self._args.input_file, "rb") as f:
			self._image = f.read()
		with contextlib.suppress(FileExistsError):
			os.makedirs(self._args.output_dir)

		for entry_no in range(64):
			data = self._image[64 * entry_no : 64 * (entry_no + 1)]
			(offset, size, name) = struct.unpack("< L L 56s", data)
			if offset != 0xffffffff:
				name = name.rstrip(b"\x00").decode()
				print("%s: offset 0x%x size %d" % (name, offset, size))
				output_filename = self._args.output_dir + "/" + name + ".raw"
				with open(output_filename, "wb") as f:
					f.write(self._image[offset : offset + size])

				output_filename_wav = self._args.output_dir + "/" + name + ".wav"
				subprocess.check_output([ "sox", "-r", "11025", "-e", "unsigned", "-b", "8", "-c", "1", "-t", "raw", output_filename, output_filename_wav ])

		json_toc = self._image[4096 : 8192].rstrip(b"\xff")
		json_toc = json.loads(json_toc)
		with open(self._args.output_dir + "/toc.json", "w") as f:
			json.dump(json_toc, f, indent = 4, sort_keys = True)
