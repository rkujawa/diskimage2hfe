#CC=gcc
CC=vc
#CFLAGS=-m32 -O2 -I. -I../common/ -I../common/includes 
CFLAGS=-O2 -I. -I../common/ -I../common/includes -DBIGENDIAN -DNEED_SWAB
#LDFLAGS=-lm -m32
LDFLAGS=-lmieee
EXEC=diskimage2hfe

ADFLIBOBJ= adf_hd.o adf_disk.o adf_raw.o adf_bitm.o adf_dump.o\
        adf_util.o adf_env.o adf_nativ.o adf_dir.o adf_file.o adf_cache.o \
        adf_link.o adf_salv.o

XDMSLIBOBJ= getbits.o crc_csum.o maketbl.o pfile.o getbits.o tables.o u_deep.o u_heavy.o u_init.o u_medium.o u_quick.o u_rle.o
ZLIBOBJ=adler32.o compress.o crc32.o deflate.o gzio.o infback.o inffast.o inflate.o inftrees.o trees.o uncompr.o zutil.o

DISKLOADERPLUGINS=adf_loader.o vfile.o dms_loader.o
# adz_loader.o ipf_loader.o libcapsimage.so.2.0 hfe_loader.o 

all: $(EXEC)

diskimage2hfe: img2hfe.o amiga_compat.o floppy_loader.o amiga_api.o floppy_utils.o amiga_track.o crc.o hfe_file_writer.o $(ADFLIBOBJ) $(DISKLOADERPLUGINS) $(XDMSLIBOBJ)
	$(CC) -o $@    $^ $(LDFLAGS)

amiga_compat.o: amiga_compat.c
	$(CC) -o $@ -c $< $(CFLAGS)

floppy_loader.o: ../common/floppy_loader.c
	$(CC) -o $@ -c $< $(CFLAGS)

floppy_utils.o: ../common/floppy_utils.c
	$(CC) -o $@ -c $< $(CFLAGS)

amiga_api.o: amiga_api.c
	$(CC) -o $@ -c $< $(CFLAGS)

usb_hxcfloppyemulator.o : ../common/usb_floppyemulator/usb_hxcfloppyemulator.c
	$(CC) -o $@ -c $< $(CFLAGS)

variablebitrate.o : ../common/usb_floppyemulator/variablebitrate.c
	$(CC) -o $@ -c $< $(CFLAGS)

hfe_file_writer.o : ../common/hfe_file_writer.c ../common/hfe_file_writer.h
	$(CC) -o $@ -c $< $(CFLAGS)

amiga_track.o: ../common/plugins/common/amiga_track.c ../common/plugins/common/amiga_track.h
	$(CC) -o $@ -c $< $(CFLAGS)

crc.o: ../common/plugins/common/crc.c ../common/plugins/common/crc.h
	$(CC) -o $@ -c $< $(CFLAGS)

iso_ibm_track.o : ../common/plugins/common/iso_ibm_track.c ../common/plugins/common/iso_ibm_track.h
	$(CC) -o $@ -c $< $(CFLAGS)

gcr_track.o : ../common/plugins/common/gcr_track.c ../common/plugins/common/gcr_track.h
	$(CC) -o $@ -c $< $(CFLAGS)

adf_loader.o : ../common/plugins/adf_loader/adf_loader.c ../common/plugins/adf_loader/adf_loader.h
	$(CC) -o $@ -c $< $(CFLAGS)

ipf_loader.o : ../common/plugins/ipf_loader/ipf_loader.c ../common/plugins/ipf_loader/ipf_loader.h
	$(CC) -o $@ -c $< $(CFLAGS)

amigadosfs_loader.o : ../common/plugins/amigadosfs_loader/amigadosfs_loader.c ../common/plugins/amigadosfs_loader/amigadosfs_loader.h
	$(CC) -o $@ -c $< $(CFLAGS)

dms_loader.o : ../common/plugins/dms_loader/dms_loader.c ../common/plugins/dms_loader/dms_loader.h
	$(CC) -o $@ -c $< $(CFLAGS)

vfile.o : ../common/libs/xdms/vfile.c ../common/libs/xdms/vfile.h
	$(CC) -o $@ -c $< $(CFLAGS)

adz_loader.o : ../common/plugins/adz_loader/adz_loader.c ../common/plugins/adz_loader/adz_loader.h
	$(CC) -o $@ -c $< $(CFLAGS)

adf_hd.o : ../common/libs/adflib/Lib/adf_hd.c
	$(CC) -o $@ -c $< $(CFLAGS)
adf_disk.o : ../common/libs/adflib/Lib/adf_disk.c
	$(CC) -o $@ -c $< $(CFLAGS)
adf_raw.o : ../common/libs/adflib/Lib/adf_raw.c
	$(CC) -o $@ -c $< $(CFLAGS)
adf_bitm.o: ../common/libs/adflib/Lib/adf_bitm.c
	$(CC) -o $@ -c $< $(CFLAGS)
adf_dump.o: ../common/libs/adflib/Lib/adf_dump.c
	$(CC) -o $@ -c $< $(CFLAGS)
adf_util.o  : ../common/libs/adflib/Lib/adf_util.c
	$(CC) -o $@ -c $< $(CFLAGS)
adf_env.o :  ../common/libs/adflib/Lib/adf_env.c
	$(CC) -o $@ -c $< $(CFLAGS)
adf_nativ.o :../common/libs/adflib/Lib/adf_nativ.c
	$(CC) -o $@ -c $< $(CFLAGS)
adf_dir.o : ../common/libs/adflib/Lib/adf_dir.c
	$(CC) -o $@ -c $< $(CFLAGS)
adf_file.o : ../common/libs/adflib/Lib/adf_file.c
	$(CC) -o $@ -c $< $(CFLAGS)
adf_cache.o : ../common/libs/adflib/Lib/adf_cache.c
	$(CC) -o $@ -c $< $(CFLAGS)
adf_link.o : ../common/libs/adflib/Lib/adf_link.c
	$(CC) -o $@ -c $< $(CFLAGS)
adf_salv.o : ../common/libs/adflib/Lib/adf_salv.c
	$(CC) -o $@ -c $< $(CFLAGS)


getbits.o : ../common/libs/xdms/xdms-1.3.2/src/getbits.c
	$(CC) -o $@ -c $< $(CFLAGS)
crc_csum.o : ../common/libs/xdms/xdms-1.3.2/src/crc_csum.c
	$(CC) -o $@ -c $< $(CFLAGS)
maketbl.o : ../common/libs/xdms/xdms-1.3.2/src/maketbl.c
	$(CC) -o $@ -c $< $(CFLAGS)
pfile.o : ../common/libs/xdms/xdms-1.3.2/src/pfile.c
	$(CC) -o $@ -c $< $(CFLAGS)
tables.o : ../common/libs/xdms/xdms-1.3.2/src/tables.c
	$(CC) -o $@ -c $< $(CFLAGS)
u_deep.o : ../common/libs/xdms/xdms-1.3.2/src/u_deep.c
	$(CC) -o $@ -c $< $(CFLAGS)
u_heavy.o : ../common/libs/xdms/xdms-1.3.2/src/u_heavy.c
	$(CC) -o $@ -c $< $(CFLAGS)
u_init.o : ../common/libs/xdms/xdms-1.3.2/src/u_init.c
	$(CC) -o $@ -c $< $(CFLAGS)
u_medium.o : ../common/libs/xdms/xdms-1.3.2/src/u_medium.c
	$(CC) -o $@ -c $< $(CFLAGS)
u_quick.o : ../common/libs/xdms/xdms-1.3.2/src/u_quick.c
	$(CC) -o $@ -c $< $(CFLAGS)
u_rle.o : ../common/libs/xdms/xdms-1.3.2/src/u_rle.c
	$(CC) -o $@ -c $< $(CFLAGS)


adler32.o : ../common/libs/zlib/zlib123/adler32.c
	$(CC) -o $@ -c $< $(CFLAGS)
compress.o : ../common/libs/zlib/zlib123/compress.c
	$(CC) -o $@ -c $< $(CFLAGS)
crc32.o : ../common/libs/zlib/zlib123/crc32.c
	$(CC) -o $@ -c $< $(CFLAGS)
deflate.o : ../common/libs/zlib/zlib123/deflate.c
	$(CC) -o $@ -c $< $(CFLAGS)
gzio.o : ../common/libs/zlib/zlib123/gzio.c
	$(CC) -o $@ -c $< $(CFLAGS)
infback.o : ../common/libs/zlib/zlib123/infback.c
	$(CC) -o $@ -c $< $(CFLAGS)
inffast.o : ../common/libs/zlib/zlib123/inffast.c
	$(CC) -o $@ -c $< $(CFLAGS)
inflate.o : ../common/libs/zlib/zlib123/inflate.c
	$(CC) -o $@ -c $< $(CFLAGS)
inftrees.o : ../common/libs/zlib/zlib123/inftrees.c
	$(CC) -o $@ -c $< $(CFLAGS)
trees.o : ../common/libs/zlib/zlib123/trees.c
	$(CC) -o $@ -c $< $(CFLAGS)
uncompr.o : ../common/libs/zlib/zlib123/uncompr.c
	$(CC) -o $@ -c $< $(CFLAGS)
zutil.o : ../common/libs/zlib/zlib123/zutil.c
	$(CC) -o $@ -c $< $(CFLAGS)


clean:
	rm -rf *.o
	rm -f $(EXEC)

.PHONY: clean
