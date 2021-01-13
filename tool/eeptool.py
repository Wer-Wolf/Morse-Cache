#!/usr/bin/python3

from argparse import ArgumentParser
import sys

__version__ = "3.0"

MAX_DIGITS = 61
MAX_DATA_LENGTH = MAX_DIGITS + 1


parser = ArgumentParser(description="Read/Create EEPROM images for the Morse-Cache (www.github.com/Wer-Wolf/Morse-Cache)")
parser.add_argument("-w", "--write", type=str, help="convert provided number sequence into a EEPROM image", metavar="NUMBERS")
parser.add_argument("file", type=str, help="file to read to/write from")
parser.add_argument("-V", "--version", help="display version number", action="version", version=f"%(prog)s {__version__}")
args = parser.parse_args()
try:
    if args.write is None:
        with open(args.file, "rb") as fd:
            damaged_entries = 0
            digits = 0
            data = fd.read(MAX_DATA_LENGTH)
            sys.stdout.write(f"Content of {args.file}: ")
            for byte in data:
                if byte == 0x00:
                    sys.stdout.write("\n")
                    break
                char = chr(byte)
                if char.isnumeric():
                    sys.stdout.write(char)
                    digits += 1
                else:
                    sys.stdout.write("?")
                    damaged_entries += 1
            else:
                sys.stdout.write("\n")
                sys.stderr.write("WARNING: Delimiter missing\n")
            print(f"Digits: {digits}")
            print(f"Damaged entries: {damaged_entries}")
            if damaged_entries > 0 or b"\x00" not in data:
                print("\nWARNING: The EEPROM image is damaged, please consider creating a new EEPROM image")
    elif not args.write.isnumeric():
        sys.exit("Only numbers (0-9) are supported")
    elif len(args.write) > MAX_DIGITS:
        sys.exit(f"Too many numbers (max is {MAX_DIGITS})")
    else:
        padding = MAX_DATA_LENGTH - len(args.write)
        with open(args.file, "w") as fd:
            fd.write(args.write + "\x00" * padding)
        print("[" + "#" * len(args.write) + "=" * padding + "]")
        print(f"{len(args.write)} numbers written")
except FileNotFoundError as msg:
    sys.exit(msg)

sys.exit(0)
