/*	$NetBSD: ns_parser.h,v 1.1.1.2 2000/11/10 09:34:27 itojun Exp $	*/

#ifndef YYERRCODE
#define YYERRCODE 256
#endif

#define L_EOS 257
#define L_IPADDR 258
#define L_NUMBER 259
#define L_STRING 260
#define L_QSTRING 261
#define L_END_INCLUDE 262
#define T_INCLUDE 263
#define T_OPTIONS 264
#define T_DIRECTORY 265
#define T_PIDFILE 266
#define T_NAMED_XFER 267
#define T_DUMP_FILE 268
#define T_STATS_FILE 269
#define T_MEMSTATS_FILE 270
#define T_FAKE_IQUERY 271
#define T_RECURSION 272
#define T_FETCH_GLUE 273
#define T_QUERY_SOURCE 274
#define T_LISTEN_ON 275
#define T_PORT 276
#define T_ADDRESS 277
#define T_RRSET_ORDER 278
#define T_ORDER 279
#define T_NAME 280
#define T_CLASS 281
#define T_CONTROLS 282
#define T_INET 283
#define T_UNIX 284
#define T_PERM 285
#define T_OWNER 286
#define T_GROUP 287
#define T_ALLOW 288
#define T_DATASIZE 289
#define T_STACKSIZE 290
#define T_CORESIZE 291
#define T_DEFAULT 292
#define T_UNLIMITED 293
#define T_FILES 294
#define T_VERSION 295
#define T_HOSTSTATS 296
#define T_DEALLOC_ON_EXIT 297
#define T_TRANSFERS_IN 298
#define T_TRANSFERS_OUT 299
#define T_TRANSFERS_PER_NS 300
#define T_TRANSFER_FORMAT 301
#define T_MAX_TRANSFER_TIME_IN 302
#define T_SERIAL_QUERIES 303
#define T_ONE_ANSWER 304
#define T_MANY_ANSWERS 305
#define T_NOTIFY 306
#define T_AUTH_NXDOMAIN 307
#define T_MULTIPLE_CNAMES 308
#define T_USE_IXFR 309
#define T_MAINTAIN_IXFR_BASE 310
#define T_CLEAN_INTERVAL 311
#define T_INTERFACE_INTERVAL 312
#define T_STATS_INTERVAL 313
#define T_MAX_LOG_SIZE_IXFR 314
#define T_HEARTBEAT 315
#define T_USE_ID_POOL 316
#define T_MAX_NCACHE_TTL 317
#define T_HAS_OLD_CLIENTS 318
#define T_RFC2308_TYPE1 319
#define T_LAME_TTL 320
#define T_MIN_ROOTS 321
#define T_TREAT_CR_AS_SPACE 322
#define T_LOGGING 323
#define T_CATEGORY 324
#define T_CHANNEL 325
#define T_SEVERITY 326
#define T_DYNAMIC 327
#define T_FILE 328
#define T_VERSIONS 329
#define T_SIZE 330
#define T_SYSLOG 331
#define T_DEBUG 332
#define T_NULL_OUTPUT 333
#define T_PRINT_TIME 334
#define T_PRINT_CATEGORY 335
#define T_PRINT_SEVERITY 336
#define T_SORTLIST 337
#define T_TOPOLOGY 338
#define T_SERVER 339
#define T_LONG_AXFR 340
#define T_BOGUS 341
#define T_TRANSFERS 342
#define T_KEYS 343
#define T_SUPPORT_IXFR 344
#define T_ZONE 345
#define T_IN 346
#define T_CHAOS 347
#define T_HESIOD 348
#define T_TYPE 349
#define T_MASTER 350
#define T_SLAVE 351
#define T_STUB 352
#define T_RESPONSE 353
#define T_HINT 354
#define T_MASTERS 355
#define T_TRANSFER_SOURCE 356
#define T_PUBKEY 357
#define T_ALSO_NOTIFY 358
#define T_DIALUP 359
#define T_FILE_IXFR 360
#define T_IXFR_TMP 361
#define T_TRUSTED_KEYS 362
#define T_ACL 363
#define T_ALLOW_UPDATE 364
#define T_ALLOW_QUERY 365
#define T_ALLOW_TRANSFER 366
#define T_ALLOW_RECURSION 367
#define T_BLACKHOLE 368
#define T_SEC_KEY 369
#define T_ALGID 370
#define T_SECRET 371
#define T_CHECK_NAMES 372
#define T_WARN 373
#define T_FAIL 374
#define T_IGNORE 375
#define T_FORWARD 376
#define T_FORWARDERS 377
#define T_ONLY 378
#define T_FIRST 379
#define T_IF_NO_ANSWER 380
#define T_IF_NO_DOMAIN 381
#define T_YES 382
#define T_TRUE 383
#define T_NO 384
#define T_FALSE 385
typedef union {
	char *			cp;
	int			s_int;
	long			num;
	u_long			ul_int;
	u_int16_t		us_int;
	struct in_addr		ip_addr;
	ip_match_element	ime;
	ip_match_list		iml;
	rrset_order_list	rol;
	rrset_order_element	roe;
	struct dst_key *	keyi;
	enum axfr_format	axfr_fmt;
} YYSTYPE;
extern YYSTYPE yylval;
