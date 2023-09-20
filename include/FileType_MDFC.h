#ifndef FILETYPE_MDFC_H
#define FILETYPE_MDFC_H

	//fafik77Lib_declarations (2020-09-12) [

enum _FileType_MDFC_storesType: unsigned short{
	_FileType_MDFC_st_none,
	_FileType_MDFC_st_simple,
		//class `encrFile_7_struct_Header` (auto key) key, size, size, front, back, (encode all previous) as mask, shuffle	(string buff only version 32-bit limit)
	_FileType_MDFC_st_encrypted_7,
		//the same as above except uses fafik77Streams instead of string-char buffers (well not really for outputBuff)
	//_FileType_MDFC_st_encrypted_7_fafikStream,

};
	///do not change order of that, always Append
enum _FileType_MDFC_Checksum: unsigned short{
	_FileType_MDFC_Checksum_none,
	_FileType_MDFC_Checksum_MD5,
	_FileType_MDFC_Checksum_SHA1,
	_FileType_MDFC_Checksum_SHA2_224,
	_FileType_MDFC_Checksum_SHA2_256,
	_FileType_MDFC_Checksum_SHA2_384,
	_FileType_MDFC_Checksum_SHA2_512,
		///2012 "Keccak" hasher that should be even 80% faster then SHA2
	_FileType_MDFC_Checksum_SHA3,
		///SHA256 based hasher by fafik77, 32-bit, v1
	_FileType_MDFC_Checksum_F77sha_32_v1,
	_FileType_MDFC_Checksum_crc32,
	_FileType_MDFC_Checksum_crc64,

	_FileType_MDFC_Checksum_SHA3_BLAKE1_224,
	_FileType_MDFC_Checksum_SHA3_BLAKE1_256,
	_FileType_MDFC_Checksum_SHA3_BLAKE1_384,
	_FileType_MDFC_Checksum_SHA3_BLAKE1_512,

	_FileType_MDFC_Checksum_SHA3_BLAKE2_224,
	_FileType_MDFC_Checksum_SHA3_BLAKE2_256,
	_FileType_MDFC_Checksum_SHA3_BLAKE2_384,
	_FileType_MDFC_Checksum_SHA3_BLAKE2_512,

		///2020 hasher 200x faster then SHA (upper limited by device IO speed, not CPU speed)
	_FileType_MDFC_Checksum_SHA3_BLAKE3,

		///16 xmodem
	_FileType_MDFC_Checksum_crc16,
	_FileType_MDFC_Checksum_MD2,
	_FileType_MDFC_Checksum_MD4,
	_FileType_MDFC_Checksum_MD6,

};
	//do not change order of that, always Append
enum _FileType_MDFC_addHeadType: unsigned int{
		//invalid additional Header Type
	_FileType_MDFC_addHeadType_none,
		//user requested the data encryption of file (with user provided key (assumed if not specified))
	_FileType_MDFC_addHeadType_UserProvidedData,
		//used for managing windows SymLink (with my private key for en-crypting Symlink)
	_FileType_MDFC_addHeadType_keyMDFCSymlink,
		//Data generated by MDFC for user
	_FileType_MDFC_addHeadType_AutoGenData,
		//File Hash generated by MDFC for user
	_FileType_MDFC_addHeadType_MDFCHash,
};


	///keep the (Header+ Hash+ any additional data before actual data)<= 512 Bytes
struct _FileType_MDFC_Header{
  //data store
	 ///'MDFC'
	const char FileType[4]= {'M','D','F','C'};
	 ///this position does matter (otherwise it adds 2 Bytes) @see _FileType_MDFC_storesType
	unsigned short storesType= _FileType_MDFC_st_none;
	 ///_FileType_MDFC_Checksum
	unsigned short checksumType= _FileType_MDFC_Checksum_none;
	BYTE storesType_version= 0;
		///@see _FileType_MDFC_additionalHeader
	BYTE additionalHeaderSize= 0;

 protected:
		///ensure this structure is 12 Bytes
	BYTE pad_12[2]= {0};
 public:
  //functions
	inline bool operator== (const _FileType_MDFC_Header& other){
		return !memcmp(this, &other, sizeof(_FileType_MDFC_Header)-sizeof(pad_12) );
	}
		///@return pos to first Byte of data that is not Header (might be Hash)
	inline size_t getHeaderEndPos()const {
		return sizeof(_FileType_MDFC_Header)+ additionalHeaderSize;
	}
	static size_t getSupposedHeaderStructSize() {
		return 12;
	}
	static size_t getSize() {
		return sizeof(_FileType_MDFC_Header);
	}
	inline bool isFileType_MDFC()const {
		_FileType_MDFC_Header other;
		return !memcmp(this->FileType, &other.FileType, sizeof(this->FileType));
	}
		///@return 0 on error, 1 on success
	inline bool setFromRawData(const void* dataIn, const size_t& dataSize){
		if(dataSize< sizeof(_FileType_MDFC_Header) ) return 0;
		memcpy(this, dataIn, sizeof(_FileType_MDFC_Header) );
		return 1;
	}
	inline const BYTE* getRaw_forExport()const {return reinterpret_cast<const BYTE*>(this);}
//		//virtual --nope(adds 4Byte size)
//		///override this for your full size
//	virtual size_t getFullHeaderSize() const {return 12;}
};

struct _FileType_MDFC_additionalHeader{
	 ///@see _FileType_MDFC_addHeadType
	_FileType_MDFC_addHeadType addHeadType= _FileType_MDFC_addHeadType_none;

	 ///@return 0 on error, 1 on success
	inline bool setFromRawData(const void* dataIn, const size_t& dataSize) {
		if(dataSize< sizeof(_FileType_MDFC_additionalHeader) ) return 0;
		memcpy(this, dataIn, sizeof(_FileType_MDFC_additionalHeader) );
		return 1;
	}
};
	// ] end fafik77Lib_declarations
#endif // FILETYPE_MDFC_H
