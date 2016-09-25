from Crypto.Cipher import AES
import hashlib
import sys

pwd = hashlib.md5()
algkey = pwd.digest()

cipher = AES.new(algkey)

def pad(s):
	return s + ((16 - len(s)% 16) * 's')

message = print(sys.argv)

def encrypt(message):
	global cipher
	return cipher.encrypt(pad(message))

