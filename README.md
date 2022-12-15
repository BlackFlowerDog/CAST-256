# CAST-256
The encryption algorithm CAST-256 implementation with CFB mode and estimation of bits destribution.
## About algorithm
CAST-256 based on earlier encryption algorithm CAST-128 and use the same elements including four S-boxes. S-box is the lookout table 8x32 bits that allows replace 8-bit number to 32-bits number.

The algorithm encrypt and decrypt information by 128-bits blocks. Key size can be 128, 160, 192, 224 or 256 bits. A number of quad-rounds is 12 that arranged in a generalized Feistel network. In each round four pairs of masking and rotate subkeys and three round-function are used.

For decryption you need reverse the order of masking and rotate subkeys and use the same algorithm which you use for encryption.
