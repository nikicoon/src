/*	$NetBSD: mcadevs_data.h,v 1.10 2001/04/19 18:38:50 jdolecek Exp $	*/

/*
 * THIS FILE AUTOMATICALLY GENERATED.  DO NOT EDIT.
 *
 * generated from:
 *	NetBSD: mcadevs,v 1.9 2001/04/19 18:38:23 jdolecek Exp 
 */

/*-
 * Copyright (c) 2000 The NetBSD Foundation, Inc.
 * Copyright (c) 1996-1999 Scott D. Telford.
 * All rights reserved.
 *
 * This code is derived from software contributed to The NetBSD Foundation
 * by Scott Telford <s.telford@ed.ac.uk>.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the NetBSD
 *	Foundation, Inc. and its contributors.
 * 4. Neither the name of The NetBSD Foundation nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE NETBSD FOUNDATION, INC. AND CONTRIBUTORS
 * ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

/*
 * List of known MCA devices
 */

const struct mca_knowndev mca_knowndevs[] = {
    { 0x002D,	"Madge Smart 16/4 MC RingNode Adapter" },
    { 0x0041,	"3Com 3C527 Ethernet Adapter" },
    { 0x004E,	"NCR 53C700 SCSI Host Adapter" },
    { 0x004F,	"NCR 53C710 SCSI Host Adapter" },
    { 0x0061,	"NCR Dual Channel Ethernet Adapter" },
    { 0x0069,	"Network Peripherals FDDI Adapter" },
    { 0x0074,	"Madge Smart 16/4 MC32 RingNode Adapter" },
    { 0x0086,	"Orchid Pro II/MC v2.0/HiColor Adapter" },
    { 0x0092,	"NCR 53C700 SCSI MCA Adapter" },
    { 0x00C4,	"Racore 16/4 Token-Ring Adapter" },
    { 0x00C5,	"Racore 16/4 Token-Ring Adapter" },
    { 0x0100,	"NCR 16/4 Token-Ring Adapter/MC" },
    { 0x0101,	"Proteon p1840 ProNET-4 Network Adapter" },
    { 0x01B2,	"Cogent E/MASTER III MCA Ethernet Adapter" },
    { 0x01BA,	"NCR 53C710 SCSI Host Adapter" },
    { 0x01BB,	"NCR 3360 SCSI Host Adapter" },
    { 0x01DB,	"Corvus Omninet1 Adapter" },
    { 0x0202,	"Proteon p1890 4/16 Token Ring Adapter" },
    { 0x0280,	"Systech 6280 Host Adapter" },
    { 0x0300,	"NCR SIMPL SCSI Host Adapter" },
    { 0x0301,	"Wichita 6298 (ADP 3702) Adapter" },
    { 0x0425,	"Superfax Modem Adapter" },
    { 0x04FF,	"Viking PS/2 Display Controller" },
    { 0x0500,	"CORE CNT-MCK Fixed Disk Adapter" },
    { 0x0708,	"BusTek BT-640A SCSI Host Adapter" },
    { 0x0803,	"Madge MCA Ring Node Adapter" },
    { 0x0808,	"BICC Isolan 4110 Ethernet Adapter" },
    { 0x0F1F,	"Adaptec AHA-1640 SCSI Adapter" },
    { 0x0F2F,	"Lantana Cypress/2 Token Ring MCA" },
    { 0x0F5F,	"GSS MC1000/Eizo GC40 Video Adapter" },
    { 0x0F7F,	"Metacomp PSconnect/PScomm4" },
    { 0x0FCC,	"D-Link Ethernet Adapter" },
    { 0x0FDF,	"MICOM-Interlan NI9210 Ethernet Adapter" },
    { 0x1185,	"Ven-Tel 2400 Modem" },
    { 0x35FF,	"Dual UART Adapter" },
    { 0x5022,	"Online CDI-210S (Sony) CD-ROM Adapter" },
    { 0x5084,	"Creative Labs Sound Blaster/MC" },
    { 0x5103,	"Creative Labs Sound Blaster Pro MCV CT5330" },
    { 0x5116,	"NCR Business Audio w/ FM Synthesis" },
    { 0x5125,	"Tecmar QT-60/90/125 Tape Drive Adapter" },
    { 0x5126,	"Tecmar Floppy Tape Adapter" },
    { 0x5136,	"NCR Audio Adapter w/o FM Synthesis" },
    { 0x5333,	"National Inst. MC-GPIB IEEE-488 Adapter" },
    { 0x5500,	"COREtape QIC-02 Tape Adapter" },
    { 0x568B,	"MaynStream QIC-02 Tape Drive Adapter" },
    { 0x568D,	"MaynStream SCSI Tape Drive Adapter" },
    { 0x57FE,	"Cipher/Archive Tape Drive Adapter" },
    { 0x5822,	"Chase Serial I/O Controller (small 4-port)" },
    { 0x5823,	"Chase Serial I/O Controller (4-port)" },
    { 0x5824,	"Chase Serial I/O Controller (8-port)" },
    { 0x5825,	"Chase Serial I/O Controller (16-port)" },
    { 0x5830,	"Axel Tape Drive Adapter" },
    { 0x5962,	"Alert Technology Serial Adapter" },
    { 0x5EEE,	"Hitachi CD-ROM Adapter" },
    { 0x5F77,	"Future Domain SCSI Adapter" },
    { 0x5FF8,	"Plus Passport MC Adapter" },
    { 0x6001,	"Tiara LANcard/E Ethernet Adapter" },
    { 0x6014,	"Standard Microsystems ARCNET-PS110 Adapter" },
    { 0x6018,	"Gateway Comms. G/Ethernet Adapter" },
    { 0x601F,	"HP Scanner Interface Adapter" },
    { 0x602F,	"Arcmaster ARCnet Adapter" },
    { 0x6042,	"3Com EtherLink/MC Ethernet Adapter (3C523)" },
    { 0x6060,	"IDEAcomm 5251 7 LU Adapter" },
    { 0x6091,	"Stallion ONboard2 Terminal Adapter" },
    { 0x609E,	"Microtek MS-PS/2 Interface Adapter" },
    { 0x60C9,	"Boca Bidirectional Parallel Adapter" },
    { 0x60D0,	"Hayes JT Fax 4800P" },
    { 0x60E5,	"Boca Dual Async/Parallel Adapter 2" },
    { 0x60ED,	"M-DCB/2 Disc Coprocessor Board" },
    { 0x6127,	"Future Domain MCS-600/700 SCSI Host Adapter" },
    { 0x613D,	"Rabbit RB75 Remote Comms. Processor" },
    { 0x613F,	"SONIC Model 50" },
    { 0x6157,	"Novell Remote III Adapter" },
    { 0x616A,	"NewPort WNIC Adapter" },
    { 0x616E,	"Computone PS6" },
    { 0x617E,	"Computone PS8 8-port Adapter" },
    { 0x61BF,	"Intel SatisFAXtion Adapter" },
    { 0x61C8,	"EtherCard PLUS Elite/A (8013EP/A)" },
    { 0x61C9,	"EtherCard PLUS Elite 10T/A (8013WP/A)" },
    { 0x61F2,	"Computone IntelliPort-II MC-8" },
    { 0x61F3,	"Computone IntelliPort-IIEX MC-CEX" },
    { 0x61FD,	"Computone PSCC Cluster Controller" },
    { 0x6201,	"Quadport PS/Q I/O Adapter" },
    { 0x6203,	"Quadport PS/Q I/O Adapter" },
    { 0x6205,	"Excelan EXOS 215T Ethernet Adapter" },
    { 0x621C,	"NCR 16-Port Serial Controller/MC" },
    { 0x621F,	"HP ScanJet II Adapter" },
    { 0x6222,	"NCI 4700 PCI/MC" },
    { 0x6262,	"Proteon p1800 ProNET-10 Network Adapter" },
    { 0x6263,	"Arco AC-1070 IDE Adapter" },
    { 0x626B,	"Apricot AdLOC Security Card" },
    { 0x627C,	"3Com 3C529 Ethernet Adapter" },
    { 0x627D,	"3Com 3C529-TP Ethernet Adapter" },
    { 0x628B,	"Intel EtherExpress 16/MC Adapter" },
    { 0x62B6,	"Arco AC-1079 SLIM Drive Adapter" },
    { 0x62DB,	"3Com 3C529 Ethernet Adapter (test mode)" },
    { 0x62F0,	"Plexus AIP2 Card" },
    { 0x62F6,	"3Com 3C529 Ethernet Adapter (10base2/T)" },
    { 0x62F7,	"3Com 3C529 Ethernet Adapter (10baseT)" },
    { 0x6361,	"Arnet Smartport/2 Communications Adapter" },
    { 0x63B3,	"CH GameCard III-Automatic/MCA Adapter" },
    { 0x63CA,	"HP 27246 Ethernet Adapter" },
    { 0x6406,	"AT&T GIS 8-Port Serial Controller/MC" },
    { 0x6407,	"Stargate Adapter" },
    { 0x6410,	"ATI AT1720T" },
    { 0x6413,	"ATI AT1720BT" },
    { 0x6416,	"ATI AT1720AT" },
    { 0x6419,	"ATI AT1720FT" },
    { 0x64B6,	"Thomas-Conrad ARC-CARD/MC" },
    { 0x64B7,	"Thomas-Conrad TCNS/MC" },
    { 0x651B,	"Racore Token-Ring Adapter" },
    { 0x651E,	"Racore LANpac II" },
    { 0x6612,	"Evercom Modem" },
    { 0x6780,	"Roalan/NeoTecH 4-Channel Serial Adapter" },
    { 0x6781,	"Roalan/NeoTecH Serial Adapter" },
    { 0x6782,	"Roalan/NeoTecH Parallel Adapter" },
    { 0x6783,	"Roalan/NeoTecH Serial/Parallel Adapter" },
    { 0x6784,	"Roalan/NeoTecH Dual RS-422/485 Serial Adapter" },
    { 0x6785,	"Roalan/NeoTecH RS-422/485 Serial Adapter" },
    { 0x6786,	"Roalan/NeoTecH DP/1A Dual Parallel Adapter" },
    { 0x6787,	"Roalan/NeoTecH MS/4A 4-Channel Serial Adapter" },
    { 0x6788,	"Roalan/NeoTecH MS/4B 4-Channel Serial Adapter" },
    { 0x6789,	"Roalan/NeoTecH MS/4C 4-Channel Serial Adapter" },
    { 0x678B,	"NeoTecH Dual RS-232 Async. Adapter" },
    { 0x678C,	"NeoTecH S2M100 Dual RS-232 Async. Adapter" },
    { 0x678D,	"NeoTecH MSP/4 4-Channel Serial/Parallel Adapter" },
    { 0x678F,	"BBS GPIB-3000 IEEE-488 Adapter" },
    { 0x6781,	"NeoTecH Single RS-232 Async. Adapter, SM110" },
    { 0x6792,	"NeoTecH SPM121/SPM321 Serial/Parallel Adapter" },
    { 0x6795,	"NeoTecH S1M200 Single RS-422/485 Async. Adapter" },
    { 0x6796,	"NeoTecH PM101 Single Bidirectional Parallel Adapter" },
    { 0x6797,	"NeoTecH PM102 Dual Bidirectional Parallel Adapter" },
    { 0x67E1,	"NCR 8-Port Serial Controller/MC" },
    { 0x68FF,	"RM Z-Net LAN Controller" },
    { 0x6A14,	"NCR WaveLAN Adapter" },
    { 0x6A21,	"Microeye 1 C Mk2 Video Digitiser" },
    { 0x6A22,	"RM Ethernet-MC/2 Adapter" },
    { 0x6A40,	"Microeye Picturebook Adapter" },
    { 0x6A5C,	"Specialix RIO Communications Adapter" },
    { 0x6A7B,	"Tueplan Dual Channel Parallel Adapter" },
    { 0x6A7F,	"RM M-Series BBC I/O Adapter" },
    { 0x6A8D,	"Quattro PS Modem" },
    { 0x6AB4,	"Mayze Syncro 24PS V.26bis Modem" },
    { 0x6AB5,	"Mayze Syncro 48PS V.27ter Modem" },
    { 0x6ABF,	"RM Ethernet MC Adapter" },
    { 0x6AE7,	"Dacom Unity Gold+/2 Modem" },
    { 0x6AFB,	"Miracom Keycard/PS Modem" },
    { 0x6AFD,	"SKNET Ethernet Adapter/G" },
    { 0x6B76,	"Network Designers Garnet 2 Adapter" },
    { 0x6B80,	"DataFlex Chameleo Modem" },
    { 0x6B95,	"CORE ESDI Fixed Disk Adapter" },
    { 0x6B9B,	"Specialix Multi-port Serial Adapter" },
    { 0x6B9C,	"HWF-MC2 Serial Communications Adapter" },
    { 0x6B9D,	"SoftWare Forge SWF-MC1 X.25 Adapter" },
    { 0x6BBA,	"Apricot On-board Ethernet Adapter" },
    { 0x6BBB,	"Barr Synchronous Communications Adapter" },
    { 0x6BBC,	"Apricot Synchronous Communications Adapter" },
    { 0x6BCB,	"CONNECT ARCnet Adapter" },
    { 0x6BE7,	"Symicron dtsx12 Token Ring Adapter" },
    { 0x6BE9,	"SKNET Ethernet Adapter" },
    { 0x6BFF,	"Torus Ethernet Adapter/MC" },
    { 0x6C0F,	"Roland MIDI Processing Unit MPU-IMC" },
    { 0x6DAC,	"Logitech ScanMan Adapter" },
    { 0x6DD4,	"FasTalk 2/V.32/42b" },
    { 0x6DD5,	"UDS Sync-Up 2/V.32/42b" },
    { 0x6DD6,	"UDS Sync-Up 2/201" },
    { 0x6DD7,	"UDS Sync-Up 2/V.26" },
    { 0x6DE0,	"UDS Sync-Up 2/V.32" },
    { 0x6DE1,	"UDS Sync-Up 2/T9628B" },
    { 0x6DE2,	"FasTalk 2/V.32" },
    { 0x6DE3,	"FasTalk 2/2400" },
    { 0x6DE4,	"UDS FasTalk 2/1200" },
    { 0x6DE5,	"UDS Sync-Up 2/V.22bis" },
    { 0x6DE6,	"UDS Sync-Up 2/201/212" },
    { 0x6DE7,	"UDS Sync-Up 2/208/201" },
    { 0x6DEF,	"DEC DE210 (DEMCA) Ethernet Adapter" },
    { 0x6E6C,	"IBM Audio Capture & Playback Adapter/A" },
    { 0x6E78,	"DCA IRMA 2-MCA Adapter" },
    { 0x6E79,	"DCA IRMA 3 3270 Adapter" },
    { 0x6E7C,	"DCA Intelligent Comm Adapter" },
    { 0x6EE7,	"IMC PCnic II Ethernet Adapter" },
    { 0x6EEE,	"UDS Sync-Up 2/Multi-Protocol Adapter" },
    { 0x6EF8,	"IRMA 3t 3270 Coax Adapter" },
    { 0x6F01,	"Hayes Smartmodem 2400P" },
    { 0x6F57,	"Octocom OSI8224APS Modem" },
    { 0x6F77,	"Lantana Tamarix/2 Ethernet Adapter" },
    { 0x6FB6,	"Canmax DLC 01 Serial Comms Adapter" },
    { 0x6FBF,	"Natural MicroSystems WATSON" },
    { 0x6FC0,	"WD EtherCard PLUS/A (WD8003E/A or WD8003ET/A)" },
    { 0x6FC1,	"WD StarCard PLUS/A (WD8003ST/A)" },
    { 0x6FC2,	"WD EtherCard PLUS 10T/A (WD8003W/A)" },
    { 0x6FE4,	"DigiBoard PS-COM/16" },
    { 0x6FE5,	"DigiBoard PS-COM/8" },
    { 0x6FE6,	"DigiBoard PS-COM/4" },
    { 0x6FE9,	"DigiBoard Open-Ender" },
    { 0x6FEA,	"DigiBoard Open-Ender RS422" },
    { 0x6FEC,	"DigiCHANNEL MC C/X Adapter" },
    { 0x6FF0,	"Arnet Multiport/2 Communications Adapter" },
    { 0x7000,	"AST Advantage/2" },
    { 0x7001,	"AST Advantage/2 w/ Dual Serial Port" },
    { 0x7002,	"AST Advantage/2 w/ Serial & Parallel Port" },
    { 0x7007,	"HyperRAM MC 32/16 Memory Expansion" },
    { 0x7010,	"Tecmar MicroRAM 386 Multifunction Board" },
    { 0x7011,	"Tecmar MicroRAM Multifunction Board" },
    { 0x7012,	"Net/One NIUps LAN Adapter" },
    { 0x7020,	"Cumulus 2Mb Memory Multifunction Adapter" },
    { 0x7021,	"Cumulus 4Mb Memory Multifunction Adapter" },
    { 0x7022,	"Cumulus 8Mb Memory Multifunction Adapter" },
    { 0x7024,	"InterQuadram QuadMEG PS8 Extended Memory/Adapter" },
    { 0x7026,	"Cumulus 2400 Baud Internal Modem" },
    { 0x7046,	"Alloy IMP8/PS Adapter" },
    { 0x7047,	"Alloy IMP2/PS Adapter" },
    { 0x7048,	"Excelogic Micromem" },
    { 0x7049,	"Micron Beyond 50/60 Memory Expansion" },
    { 0x704E,	"Video Capture Adapter/A" },
    { 0x704F,	"NCR MPCA/MC Adapter" },
    { 0x7050,	"AST RampagePlus/MC Memory Expansion" },
    { 0x7064,	"AW RCP Adapter" },
    { 0x706F,	"Aox MicroMASTER 386" },
    { 0x707D,	"Corollary 8x4mc" },
    { 0x707E,	"Irwin 4100MC Controller" },
    { 0x708E,	"Kingston KTM-8000/286 Memory Expansion" },
    { 0x708F,	"Kingston KTM-8000/386 Memory Expansion" },
    { 0x70B0,	"Intel AboveBoard MC32" },
    { 0x70D0,	"Kingston KTM-16000/386 Memory Expansion" },
    { 0x70D4,	"Kingston KTM-609/16 Memory Expansion" },
    { 0x7151,	"Novell NE/2-32 Ethernet Adapter" },
    { 0x7154,	"Novell NE/2 Ethernet Adapter" },
    { 0x7188,	"Mountain QIC-02 Tape Adapter" },
    { 0x71D4,	"Kingston DataCard 16 Adapter" },
    { 0x72F3,	"HyperRAM MC 32/16 SIMM-MF Memory Expansion" },
    { 0x7411,	"Quadboard PS/Q Multifunction Adapter" },
    { 0x7422,	"Quadboard PS/Q Multifunction Adapter" },
    { 0x7520,	"ALR MicroChannel AT Embedded Adapter" },
    { 0x7522,	"ALR MicroChannel AT Embedded Adapter" },
    { 0x76DA,	"Quadmeg PS/Q Memory Adapter" },
    { 0x76DE,	"Quadmeg PS/Q Memory Adapter" },
    { 0x7788,	"Intel AboveBoard 2 Plus" },
    { 0x77FB,	"Everex EV-136 4Mb Memory Expansion" },
    { 0x77FF,	"Alloy HI-2 PC Expansion Bus Interface" },
    { 0x7A7A,	"Boca BOCARAM/2 PLUS Memory Expansion" },
    { 0x7D7F,	"Orchid RAMQuest Extra 16/32" },
    { 0x7E76,	"Overland XL/2 9-Track Tape Coupler" },
    { 0x7F4C,	"NCR 53C94 SCSI Host Adapter" },
    { 0x7F4D,	"NCR 3421 SCSI Host Adapter" },
    { 0x7F4E,	"NCR SCSI Host Adapter" },
    { 0x7F4F,	"NCR SCSI Host Adapter" },
    { 0x7F99,	"DigiCHANNEL MC/4i or MC/8i" },
    { 0x7FBE,	"FTG PXL-480 Hi-Res Lightpen Adapter" },
    { 0x7FCE,	"Connect Tech Intellicon MC8 (Canmax CS-6060MCA)" },
    { 0x7FD5,	"NCI 4700 PCI/MC" },
    { 0x7FE9,	"DigiBoard Open-Ender DOS" },
    { 0x7FEA,	"DigiBoard MC/Xi DOS RS422" },
    { 0x7FF9,	"Iomega PC4 SCSI Host Adapter" },
    { 0x7FFA,	"Iomega PC4B-50 SCSI Host Adapter" },
    { 0x8002,	"Ahead VGA Enhancer-Z Rev. A" },
    { 0x8009,	"Vermont COBRA/2" },
    { 0x800D,	"Pixelworks PS/2 Ultra Clipper Graphics Subsystem" },
    { 0x8010,	"Microeye 1C Video Digitiser" },
    { 0x8016,	"ETAP NEFTIS/20 or ATRIS/20 Display Adapter" },
    { 0x803F,	"RM High Resolution Mono Display Adapter" },
    { 0x8044,	"Paradise 8514/A Plus Video Adapter" },
    { 0x8057,	"Digithurst Video Output" },
    { 0x8088,	"ATI 8514/Ultra Display Adapter" },
    { 0x8089,	"ATI Graphics Ultra MCA Video Accelerator" },
    { 0x80A5,	"Cornerstone SinglePage XL Display" },
    { 0x80B3,	"M-Motion Video Adapter/A" },
    { 0x80C3,	"Artist XJS MC" },
    { 0x80E6,	"RasterOps ColorBoard 1024MC" },
    { 0x80E8,	"Matrox MG-104/108-M Video Adapter" },
    { 0x80EC,	"Matrox MGA Video Adapter" },
    { 0x80F6,	"RasterOps ColorBoard 1024MC" },
    { 0x812E,	"NCR 3350/3355 HPG Video Adapter (w/o ROM)" },
    { 0x8202,	"Ahead VGA Enhancer-Z Rev. A" },
    { 0x8EF8,	"IBM Expansion Unit SCSI Adapter" },
    { 0x8EFC,	"IBM Fast SCSI-2 Adapter" },
    { 0x8EFD,	"IBM 7568 SCSI Adapter w/cache" },
    { 0x8EFE,	"IBM SCSI Adapter" },
    { 0x8EFF,	"IBM SCSI Adapter w/cache" },
    { 0x8FA0,	"IBM LANStreamer MC 32 Adapter" },
    { 0x8FC8,	"IBM Token Ring 16/4 Busmaster Server Adapter/A" },
    { 0x8FD6,	"IBM Audiovation Adapter" },
    { 0x8FD8,	"IBM XGA/XGA-2 Display Adapter" },
    { 0x8FD9,	"IBM XGA/XGA-2 Display Adapter" },
    { 0x8FDA,	"IBM XGA/XGA-2 Display Adapter" },
    { 0x8FDB,	"IBM XGA/XGA-2 Display Adapter" },
    { 0xDDFF,	"IBM ESDI Fixed Disk Controller" },
    { 0xDEFF,	"IBM Multi-Protocol Communications Adapter" },
    { 0xDF7F,	"IBM 3363 Optical Disk Adapter" },
    { 0xDF9F,	"IBM Integ. ESDI Fixed Disk & Controller" },
    { 0xDFBF,	"IBM 6157 Streaming Tape Adapter" },
    { 0xDFFA,	"IBM 5.25in Diskette Adapter/A" },
    { 0xDFFD,	"IBM ST506 Fixed Disk Adapter" },
    { 0xE000,	"IBM Token Ring Adapter/A" },
    { 0xE001,	"IBM Token Ring 16/4 Adapter/A" },
    { 0xE016,	"IBM 300/1200/2400 Internal Modem/A" },
    { 0xE04E,	"IBM High Speed Adapter/A" },
    { 0xE04F,	"IBM 3119 Adapter/A" },
    { 0xE1FF,	"IBM 3270 Connection Version B" },
    { 0xE7FF,	"IBM 3270 Connection Version A" },
    { 0xE9FF,	"IBM Expansion Unit Model 002" },
    { 0xEDFF,	"IBM Internal Modem" },
    { 0xEEBB,	"Miracom OEM Modem" },
    { 0xEEE7,	"IBM Serial Port 2" },
    { 0xEEFF,	"IBM Dual Async Adapter" },
    { 0xEF7F,	"IBM 8514/A Display Adapter" },
    { 0xEFCF,	"IBM Store Loop Adapter/A" },
    { 0xEFD4,	"IBM PS/2 Adapter/A for Ethernet Networks (UTP)" },
    { 0xEFD5,	"IBM PS/2 Adapter/A for Ethernet Networks (BNC)" },
    { 0xEFDC,	"Intel DS2 Adapter" },
    { 0xEFDF,	"ARCnet ASTER/2 Adapter" },
    { 0xEFE5,	"IBM PS/2 Adapter/A for Ethernet Networks" },
    { 0xEFEF,	"IBM PC Network Adapter" },
    { 0xEFF0,	"IBM Realtime I/f Co-proc. Multiport/2 or X.25/2" },
    { 0xEFF5,	"UB NIC-PS/2 Ethernet Adapter" },
    { 0xEFF6,	"IBM PagePrinter Adapter/2" },
    { 0xEFFD,	"IBM VGA Adapter" },
    { 0xF04F,	"IBM 3117 Scanner Adapter/A" },
    { 0xF7F7,	"IBM 2-8Mb 80286 Memory Expansion" },
    { 0xF7FE,	"IBM Expanded Memory" },
    { 0xFAFF,	"IBM 32-bit Memory Expansion" },
    { 0xFCFF,	"IBM Memory Expansion" },
    { 0xFDDE,	"IBM Enhanced 80386 Memory Expansion w/ROM" },
    { 0xFDDF,	"IBM Enhanced 80386 Memory Expansion" },
    { 0xFEFE,	"IBM 2Mb 16-bit Memory Adapter" },
    { 0xFFF8,	"IBM 5250 Workstation Emulation Adapter" },
    { 0, NULL, }
};
