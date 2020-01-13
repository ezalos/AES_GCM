#include <stdio.h>
#include <gcrypt.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <gcrypt.h>

// Compilation :
// 					gcc srcs/main.c -I/usr/include -lgcrypt

#define NEED_LIBGCRYPT_VERSION		"1.0.0"
#define ITERATIONS					10
#define IV_LEN						96
#define TAG_LEN						16
#define SALT_LEN					16


#define PROTECT_GCRYPT(ERR, X)	if (ERR = X)\
									printf("\nFailure %s\n\tReason : %s\n",\
									#X, gcry_strerror(ERR))

unsigned char TEST_KEY[] = {0x60,0x3d,0xeb,0x10,0x15,0xca,0x71,0xbe,0x2b,0x73,\
	0xae,0xf0,0x85,0x7d,0x77,0x81,0x1f,0x35,0x2c,0x07,0x3b,0x61,0x08,0xd7,0x2d,\
	0x98,0x10,0xa3,0x09,0x14,0xdf,0xf4};
unsigned char TEST_IV[] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,\
	0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
unsigned char TEST_PLAINTEXT_1[] = {0x6b,0xc1,0xbe,0xe2,0x2e,0x40,0x9f,0x96,\
	0xe9,0x3d,0x7e,0x11,0x73,0x93,0x17,0x2a};

unsigned char out_cipher[16] = {0};

typedef struct		s_pass
{
	void			*passphrase;
	size_t			passphrase_len;

	void			*key;
	size_t			key_len;

	void			*salt;
	size_t			salt_len;

}					t_pass;


typedef struct		s_crypt
{
	t_pass				pass;
	gcry_cipher_hd_t	hd;

	void			*IV;
	size_t			IV_len;

	char			*tag;
	size_t			tag_len /*= 16*/;

	void			*plaintext;
	size_t			plaintext_len;

	void			*cryptedtext;
	size_t			cryptedtext_len;
}					t_crypt;

void	print_data(char *label, char *data, int len)
{
	int		i;

	printf("\n%s :", label);
	printf("\t");
	i = -1;
	while (++i < len)
		printf("%02hhx ", data[i]);
	printf("\n");
	printf("\t\t");
	i = -1;
	while (++i < len)
		printf("%c  ", data[i]);
	printf("\n");
}

int		open_libgcrypt(void)
{
	char		*req_version;
	const char	*v_return;

	v_return = gcry_check_version(req_version);
	printf("NEED_LIBGCRYPT_VERSION :\t%s\n", NEED_LIBGCRYPT_VERSION);

	/* Version check should be the very first call because it
	makes sure that important subsystems are initialized.
	#define NEED_LIBGCRYPT_VERSION to the minimum required version. */
	if (!(v_return = gcry_check_version (NEED_LIBGCRYPT_VERSION)))
	{
		fprintf (stderr, "libgcrypt is too old (need %s, have %s)\n",
		NEED_LIBGCRYPT_VERSION, gcry_check_version (NULL));
		exit (2);
	}
	printf("Actual version is :\t\t%s\n", v_return);
	/* Disable secure memory. */
	gcry_control (GCRYCTL_DISABLE_SECMEM, 0);
	/* ... If required, other initialization goes here. */
	/* Tell Libgcrypt that initialization has completed. */
	gcry_control (GCRYCTL_INITIALIZATION_FINISHED, 0);
	return (0);
}

int		AES_GCM_setup(gcry_cipher_hd_t *hd)
{
	int		ret;
	PROTECT_GCRYPT(ret, gcry_cipher_open (hd, GCRY_CIPHER_AES128,\
		GCRY_CIPHER_MODE_GCM, 0));
	PROTECT_GCRYPT(ret, gcry_cipher_setkey(*hd, TEST_KEY, 32));
	PROTECT_GCRYPT(ret, gcry_cipher_setiv(*hd, TEST_IV, 16));
	PROTECT_GCRYPT(ret, gcry_cipher_authenticate(*hd, NULL, 0));
}

int		Key_Derivation_Function(t_crypt *cypher)
{
	int		ret;

	PROTECT_GCRYPT(ret, gcry_kdf_derive (cypher->pass.passphrase,\
		cypher->pass.passphrase_len, GCRY_KDF_PBKDF2, GCRY_MD_SHA256,\
		cypher->pass.salt, cypher->pass.salt_len, ITERATIONS,\
		cypher->pass.key_len, cypher->pass.key));
}

void	init()
{
	// open lib
	// set algo
}

void	new_crypt()
{
	//set rand data
	//crypt
	//get tag
	//create package
	//save it ?
}

int		AES_GCM_fill_lengths(t_crypt *cypher, char *passphrase)
{
	cypher->pass.key_len = gcry_cipher_get_algo_keylen(GCRY_CIPHER_AES128);
	printf("KEY LEN : %ld\n", cypher->pass.key_len);
	cypher->IV_len = 96;
	cypher->tag_len = 16;
	cypher->pass.salt_len = 16;
	cypher->pass.passphrase = passphrase;
	cypher->pass.passphrase_len = strlen(cypher->pass.passphrase);
}

int		AES_GCM_fill_data(t_crypt *cypher)
{
	// void * gcry_random_bytes ( size, GCRY_WEAK_RANDOM)
	// void gcry_create_nonce (unsigned char *buffer, size t length)
	cypher->pass.key = gcry_malloc_secure(cypher->pass.key_len);
	cypher->IV = gcry_random_bytes(cypher->IV_len, GCRY_WEAK_RANDOM);
	cypher->tag = gcry_malloc(cypher->tag_len);
	cypher->pass.salt = gcry_random_bytes(cypher->pass.salt_len, GCRY_WEAK_RANDOM);
	Key_Derivation_Function(cypher);
	cypher->cryptedtext = gcry_malloc(cypher->cryptedtext_len);
	// cypher->pass.passphrase = malloc(cypher->pass.passphrase_len);
}

int		main(int ac, char **av)
{
	t_crypt		cypher;
	int			ret;

	if (ac != 2)
		return (0);
	open_libgcrypt();
	AES_GCM_setup(&cypher.hd);
	AES_GCM_fill_lengths(&cypher, av[1]);
	AES_GCM_fill_data(&cypher);

	int		len = strlen(av[1]);
	int		crypt_size = len;
	int		uncrypt_size = len;
	char	*cryptage;
	char	*decryptage;
	char	tag[16];

	print_data("PASSPHRASE", cypher.pass.passphrase, cypher.pass.passphrase_len);
	print_data("SECRET KEY", cypher.pass.key, cypher.pass.key_len);
	// cryptage = malloc(crypt_size /*in mean needs +16 size out*/ /*+ IV_size + Salt_size + ...*/);
	// printf("%p\n", cryptage);

	// PROTECT_GCRYPT(ret, gcry_cipher_encrypt(cypher.hd, cypher.cryptedtext,
		// cypher.cryptedtext_len, cypher.plaintext, cypher.plaintext_len));
	// PROTECT_GCRYPT(ret, gcry_cipher_gettag(cypher.hd, cypher.tag, cypher.tag_len));

	// print_data("Original", av[1], len);
	// print_data("Tag    ", tag, 16);
	// print_data("Crypted", cryptage, crypt_size);

	// decryptage = malloc(uncrypt_size);

	// PROTECT_GCRYPT(ret, gcry_cipher_decrypt (hd, decryptage, uncrypt_size, cryptage, crypt_size));
	// decryptage[uncrypt_size] = 0;

	// gcry_error_t gcry_cipher_checktag (gcry cipher hd t h, const void *tag, size t taglen);

	// print_data("Uncrypte", decryptage, uncrypt_size);

	// gcry_cipher_close (hd);
	return (0);
}
