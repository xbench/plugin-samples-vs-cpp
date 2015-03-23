#ifdef XBENCHQAPLUGINSAMPLE_EXPORTS
#ifdef _WIN64
#define XBENCHQA_API __declspec(dllexport)
#else
#define XBENCHQA_API
#endif
#else
#define XBENCHQA_API __declspec(dllimport)
#endif

typedef struct _TQAPluginResultOptions {
	WORD StructVersion;
	BOOL Sorted;
	BOOL Groupable;
} TQAPluginResultOptions, *PQAPluginResultOptions;

typedef struct _TQAPluginResult {
	PWSTR Text;
	DWORD SegId;
	PQAPluginResultOptions Options;
} TQAPluginResult, *PQAPluginResult;

typedef struct _TQAPluginDeclareInfo {
	WORD StructVersion;
	WCHAR Description[256];
	WCHAR Keywords[256];
	WORD Version;
	LANGID SourceLang;
	LANGID TargetLang;
	BOOL AllowsUnicode;
	BOOL AllowsAnsi;
} TQAPluginDeclareInfo, *PQAPluginDeclareInfo;

typedef struct _TQASegmentInfo {
	PSTR Source;
	PSTR Target;
	PWSTR WSource;
	PWSTR WTarget;
	DWORD SegId;
} TQASegmentInfo, *PQASegmentInfo;

typedef void *PQAFunctionParams;

extern "C" {
	XBENCHQA_API PWSTR __stdcall GetDeclareName(void);
	XBENCHQA_API DWORD __stdcall GetFirstFunction(void);
	XBENCHQA_API DWORD __stdcall GetNextFunction(void);
	XBENCHQA_API PQAPluginDeclareInfo __stdcall GetDeclareInfo(void);
	XBENCHQA_API void _stdcall SetConfigFile(PSTR aFile);
	XBENCHQA_API void _stdcall ProcessBegin(DWORD aHandle, 
		                                    PQAFunctionParams aParams);
	XBENCHQA_API void _stdcall ProcessEnd(DWORD aHandle);
	XBENCHQA_API PQAPluginResult _stdcall ProcessSegment(DWORD aHandle, 
		                                                 TQASegmentInfo aSegInfo);
	XBENCHQA_API PQAPluginResult _stdcall GetFirstResult(DWORD aHandle);
	XBENCHQA_API PQAPluginResult _stdcall GetNextResult(DWORD aHandle);
	XBENCHQA_API PSTR _stdcall GetFunctionName(DWORD aHandle);
}