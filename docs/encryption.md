# Encryption and authentication

When creating a user, the barrier operator may pick from a number of OpenSSL EVP key exchange methods supported by gensokyonet (at the time of writing, I only have plans for X25519) for authentication. The user should keep track of their respective keypair or relevant secret data. Refreshing a user's keypair due to forgetting 💀 implies a breach of trust, so it should be negotiated with the barrier operator.

Authentication also implies the establishment of an encrypted connection, the cipher of which is determined in this header and can be picked freely by the user (again, only one algorithm is actually planned for now, which is AES-GCM for its extra auth capability).