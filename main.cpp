#include <cryptopp/cryptlib.h>
#include <cryptopp/rijndael.h>
#include <cryptopp/modes.h>
#include <cryptopp/files.h>
#include <cryptopp/osrng.h>
#include <cryptopp/hex.h>
#include <cryptopp/filters.h>

#include <iostream>
#include <string>
#include <fstream>

using namespace CryptoPP;

std::string getCipheredText(std::string plain, SecByteBlock key, SecByteBlock iv);
std::string recoverCipheredText(std::string cipher, SecByteBlock key, SecByteBlock iv);

int main(){
    AutoSeededRandomPool prng;

    SecByteBlock key(AES::DEFAULT_KEYLENGTH);
    SecByteBlock iv(AES::BLOCKSIZE);

    while(true){
        std::cout << "Would you like to:" << '\n';
        std::cout << "1. Encrypt File" << '\n';
        std::cout << "2. Decrypt File (must be encrypted by this tool)" << '\n';

        int option;
        std::cin >> option;

        if(option == 1){
            prng.GenerateBlock(key, key.size());
            prng.GenerateBlock(iv, iv.size());

            std::ofstream keyFile("key_iv.txt");
            HexEncoder encoder(new FileSink(keyFile));
            encoder.Put(key, key.size());
            keyFile << '\n';
            encoder.Put(iv, iv.size());
            encoder.MessageEnd();
            keyFile.close();

            std::string filename;
            std::cout << "Enter the name of the file to encrypt: ";
            std::cin >> filename;

            std::ifstream plainFile(filename);
            size_t dotPos = filename.rfind('.');
            std::string baseFilename = filename.substr(0, dotPos);
            std::ofstream cipheredFile(baseFilename + "_encrypted.txt");

            std::string line;
            while(std::getline(plainFile, line)){
                std::string cipher = getCipheredText(line, key, iv);

                std::string hexCipher;
                StringSource(cipher, true, new HexEncoder(new StringSink(hexCipher)));

                cipheredFile << hexCipher << '\n';
            }

            std::cout << "Successfully encrypted!" << '\n';
            return 0;
        }

        else if(option == 2){
            std::ifstream keyFile("key_iv.txt");
            std::string keyHex, ivHex;
            std::getline(keyFile, keyHex);
            std::getline(keyFile, ivHex);
            keyFile.close();

            StringSource(keyHex, true, new HexDecoder(new ArraySink(key, key.size())));
            StringSource(ivHex, true, new HexDecoder(new ArraySink(iv, iv.size())));

            std::string filename;
            std::cout << "Enter the name of the file to decrypt: ";
            std::cin >> filename;

            std::ifstream cipheredFile(filename);
            size_t dotPos = filename.rfind('_');
            std::string baseFilename = filename.substr(0, dotPos);
            std::ofstream recoveredFile(baseFilename + "_decrypted.txt");

            std::string line;
            while(std::getline(cipheredFile, line)){
                std::string decodedCipher;
                StringSource(line, true, new HexDecoder(new StringSink(decodedCipher)));

                std::string recovered = recoverCipheredText(decodedCipher, key, iv);
                recoveredFile << recovered << '\n';
            }

            std::cout << "Successfully decrypted!" << '\n';
            return 0;
        }

        else{
            std::cout << "Invalid input." << '\n';
            continue;
        }
    }
}


std::string getCipheredText(std::string plain, SecByteBlock key, SecByteBlock iv){
    std::string cipher;

    try{
        CBC_Mode<AES>::Encryption e;
        e.SetKeyWithIV(key, key.size(), iv);

        StringSource s(plain, true,
            new StreamTransformationFilter(e,
                new StringSink(cipher)
            )
        );
    }

    catch(const Exception& e){
        std::cerr << e.what() << '\n';
        exit(1);
    }

    return cipher;
}


std::string recoverCipheredText(std::string cipher, SecByteBlock key, SecByteBlock iv){
    std::string recovered;

    try{
        CBC_Mode<AES>::Decryption d;
        d.SetKeyWithIV(key, key.size(), iv);

        StringSource s(cipher, true,
            new StreamTransformationFilter(d,
                new StringSink(recovered)
            )
        );
    }

    catch(const Exception& e){
        std::cerr << e.what() << '\n';
        exit(1);
    }

    return recovered;
}
