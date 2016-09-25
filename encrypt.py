from Crypto.Cipher import AES
import hashlib

pwd = hashlib.md5()
algkey = pwd.digest()

cipher = AES.new(algkey)

def pad(s):
	return s + ((16 - len(s)% 16) * 's')

def encrypt(message):
	global cipher
	return cipher.encrypt(pad(message))

def decrypt(emessage):
	global cipher
	dec = cipher.decrypt(emessage).decode(utf-8)
	l = dec.count('{')
	return dec[:len(dec)-1]

encrypted = encrypt(message)
decrypted = decrypt(emessage)
