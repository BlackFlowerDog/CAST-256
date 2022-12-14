# CAST-256
The encryption algorithm CAST-256 implementation in the CFB mode and estimation of bits distribution.
## About algorithm
CAST-256 based on earlier encryption algorithm CAST-128 and use the same elements including four S-boxes. S-box is the lookout table 8x32 bits that allows replace 8-bit number to 32-bits number.

The algorithm encrypt and decrypt information by 128-bits blocks. Key size can be 128, 160, 192, 224 or 256 bits. A number of quad-rounds is 12 that arranged in a generalized Feistel network. In each round four pairs of masking and rotate subkeys and three round-function are used.

For decryption you need reverse the order of masking and rotate subkeys and use the same algorithm as for encryption.

## About CFB mode
### Encryption
In the CFB mode, the xor operation will be applied to the first block of input data and the cipher initialization vector. Result will be the first ciphertext block. Next, the second cipher block will be the xor operation result of second input data block and cipher the first cipher output block. Subsequent blocks are encrypted similarly.    

![encrypt](https://user-images.githubusercontent.com/115879518/207753109-29fe19d6-13fe-45fc-8ca1-328a6697924d.png)

### Decryption

To decrypt, the initialization vector is encrypted, then the XOR operation is applied to it and the first block of the ciphertext. Then each subsequent
the ciphertext block is encrypted and the XOR operation is applied to it and the next ciphertext block. The result will be plaintext.

![decrypt](https://user-images.githubusercontent.com/115879518/207753206-3a1eb112-b75f-4d03-ab86-fe719c154256.png)

## About the current implementation

### Algorithm

Starting from the command shell.
```
./a.out data.txt encrypt.txt -t -e -y
```
The first argument is the name of file with input data.

The second argument is the name of file with output data.

The third argument is a file type: text or BMP-image ("-t" or "-i").

The fourth argument is an operating mode: encryption or decryption ("-e" or "-d").

The fifth argument is an operating mode: encryption/decryption with CFB (yes) or without (no) ("-y" or "-n").

Key is on a separate file "key.txt" that in the same directory as main. If it's size is not 128, 160, 192, 224 or 256 bits the program will terminate with the error. If number of input bits is not a multiple of 128, then the required number of characters will be added to the end to fulfill this condition.

### Distribution estimation

Derivation of correlation estimates and distribution of zeros and ones:

![image](https://user-images.githubusercontent.com/115879518/207757044-6c4d8bd8-98db-433b-b2ad-a5188a6e215c.png)

Distribution of zeros and ones shows the frequency of their occurrence. Good if it will be approximately 0.5 for ciphertext.
Correlation determines the relationship between plaintext and ciphertext. Good if it will be approximately 0.
