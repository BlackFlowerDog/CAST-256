# CAST-256
The encryption algorithm CAST-256 implementation with CFB mode and estimation of bits destribution.
## About algorithm
CAST-256 based on earlier encryption algorithm CAST-128 and use the same elements including four S-boxes. S-box is the lookout table 8x32 bits that allows replace 8-bit number to 32-bits number.

The algorithm encrypt and decrypt information by 128-bits blocks. Key size can be 128, 160, 192, 224 or 256 bits. A number of quad-rounds is 12 that arranged in a generalized Feistel network. In each round four pairs of masking and rotate subkeys and three round-function are used.

For decryption you need reverse the order of masking and rotate subkeys and use the same algorithm which you use for encryption.

## About CFB mode

## About current implementation
Starting from the command shell.
```
./a.out data.txt encrypt.txt -t -e -y
```
The first argument is the name of file with input data.

The second argument is the name of file with output data.

The third argument is a file type: text or BMP-image ("-t" or "-i").

The fourth argument is an operating mode: encryption or decryption ("-e" or "-d").

The fourth argument is an operating mode: encryption/decryption with CFB (yes) or without (no) ("-y" or "-n").

