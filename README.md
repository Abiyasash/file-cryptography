# file-cryptography

This is a tool that I made using C++ and an external library of it called Crypto++. This project lets you encrypt and decrypt text in a file, and it encrypts and decrypts the text differently each time. This was made using Crypto++, an external library used for encrypting and decrypting text.

You can install Crypto++ like this:

Linux:
<code>
  sudo apt update
  sudo apt install libcrypto++-dev libcrypto++-doc libcrypto++-utils
</code>

Mac (with homebrew):
<code>
  brew install cryptopp
</code>

Windows:
Go to cryptopp.com and download the zip file and extract it.

To compile this program, you would have to add a little bit more to the command.

<code>
  g++ main.cpp -lcryptopp -o main
</code>

You have to add -lcryptopp to the middle of it.

Thanks for reading!
