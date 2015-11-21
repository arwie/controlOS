#ifndef AMCS_H_
#define AMCS_H_


#define NO_STRING_TYPE		0		/* No type */
#define ASCII8_STRING_TYPE	1		/* ASCII-8 type */
#define UTF8_STRING_TYPE 	2		/* UTF-8 type */


extern "C" {

	struct SYS_STRING;

	extern int str_GetNumBytes (SYS_STRING* string);
	extern unsigned char* str_GetData(SYS_STRING* string);
	extern SYS_STRING* str_GetString(unsigned char* data, int data_size, int type);

}



static inline string amcsGetString(SYS_STRING* mcString)
{
	return string((const char*)str_GetData(mcString));
}


//static inline SYS_STRING* amcsNewMcString(const string& str)
//{
//	auto mcString = str_GetString((unsigned char*)str.c_str(), str.length(), ASCII8_STRING_TYPE);
//
//	if (mcString == NULL)
//		throw exception();
//
//	return mcString;
//}

#endif /* AMCS_H_ */
