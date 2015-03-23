#include "stdafx.h"
#include "XbenchQAPluginSample.h"
#include <string>

//---------------------------------------------------------------------------//
// Internal implementation typedefs                                          //
//---------------------------------------------------------------------------//

typedef struct _TLongestResult {
	size_t length;
	DWORD segment_id;
} TLongestResult;

//---------------------------------------------------------------------------//
// Global constants                                                          //
//---------------------------------------------------------------------------//

const int kLongestCount = 3; // Number of longest results to show

// List of function handles in this module. Because of the way
// GetFirstFunction and GetNextFuntion are implemented in this example, they
// must be consecutive numbers and kFunctionCount must hold the total count
// of handles
const int kCheckSuspiciousHandle = 1;
const int kCheckLongestHandle = 2;
const int kFunctionCount = 2;

//---------------------------------------------------------------------------//
// Global variables                                                          //
//---------------------------------------------------------------------------//

DWORD g_function_index;

TLongestResult g_longest[kLongestCount];
DWORD g_longest_index;

//---------------------------------------------------------------------------//
// Check suspicious length.  If source text is too long compared to target   //
// or viceversa, return the error message, otherwise (no error), return NULL //
//---------------------------------------------------------------------------//
wchar_t * CheckSuspiciousLength(const size_t aSourceLength,
								const size_t aTargetLength)
{
	if (aSourceLength > 60) {
		if (aTargetLength > aSourceLength * 1.8)
			return L"Target 80% larger than Source";
	} else if (aSourceLength > 45) {
		if (aTargetLength > aSourceLength * 2.2)
			return L"Target 120% larger than Source";
	} else if (aSourceLength > 30) {
		if (aTargetLength > aSourceLength * 2.6)
			return L"Target 160% larger than Source";
	} else if (aSourceLength > 15) {
		if (aTargetLength > aSourceLength * 3)
			return L"Target 200% larger than Source";
	}

	if (aTargetLength > 60)	{
		if (aSourceLength > aTargetLength * 1.8)
			return L"Source 80% larger than Target";
	} else if (aTargetLength > 45) {
		if (aSourceLength > aTargetLength * 2.2)
			return L"Source 120% larger than Target";
	} else if (aTargetLength > 30) {
		if (aSourceLength > aTargetLength * 2.6)
			return L"Source 160% larger than Target";
	} else if (aTargetLength > 15) {
		if (aSourceLength > aTargetLength * 3)
			return L"Source 200% larger than Target";
	}

	return NULL;
}

//---------------------------------------------------------------------------//
// Check longest. Keep a global array with the longest target segments.      //
//---------------------------------------------------------------------------//
void CheckLongest(const size_t aTargetLength, const DWORD aSegId)
{
	if (aTargetLength > g_longest[kLongestCount - 1].length)
	{
		g_longest[kLongestCount - 1].length = aTargetLength;
		g_longest[kLongestCount - 1].segment_id = aSegId;

		for (int i = kLongestCount - 2;
			 (i >= 0) && (g_longest[i + 1].length > g_longest[i].length);
			 i--)
		{
			TLongestResult aux = g_longest[i];
			g_longest[i] = g_longest[i + 1];
			g_longest[i + 1] = aux;
		}
	}
}

//---------------------------------------------------------------------------//
// GetDeclareInfo. Callback called once at DLL load to provide information   //
// about the DLL.                                                            //
//---------------------------------------------------------------------------//
XBENCHQA_API PQAPluginDeclareInfo __stdcall GetDeclareInfo(void)
{
	static TQAPluginDeclareInfo plugin_declare_info;

	plugin_declare_info.StructVersion = 0;

	wcscpy_s(plugin_declare_info.Description, L"Sample plugin for Xbench");
	wcscpy_s(plugin_declare_info.Keywords, L"Sample, Example, Plugin");

	plugin_declare_info.Version = 0x0100;
	plugin_declare_info.SourceLang = 0;
	plugin_declare_info.TargetLang = 0;
	plugin_declare_info.AllowsUnicode = true;
	plugin_declare_info.AllowsAnsi = true;

	return &plugin_declare_info;
}

//---------------------------------------------------------------------------//
// GetDeclareName.  Callback called once at DLL load.  The name specified    //
// here appears in the Group Check list box of Xbench QA tab                 //
//---------------------------------------------------------------------------//
XBENCHQA_API PWSTR __stdcall GetDeclareName(void)
{
	return L"Sample Plugin";
}

//---------------------------------------------------------------------------//
// GetFunctionName.  Callback called once at DLL load. The name specified    //
// here for each aHandle appears on the List of Checks listbox of the Xbench //
// QA tab when this plugin is selected.                                      //
//                                                                           //
// For this example we have two functions, so it will show two strings.      //
//---------------------------------------------------------------------------//
XBENCHQA_API PSTR __stdcall GetFunctionName(DWORD aHandle)
{
	switch (aHandle)
	{
		case kCheckSuspiciousHandle: return "Suspicious length";
		case kCheckLongestHandle: return "Show 3 longest";
		default: return "";
	}
}

//---------------------------------------------------------------------------//
// GetFirstFunction.  Callback called once at DLL load.  It returns the      //
// first handle of the exposed QA functions, typically 1.                    //
//---------------------------------------------------------------------------//
XBENCHQA_API DWORD __stdcall GetFirstFunction(void)
{
	return g_function_index = 1;
}

//---------------------------------------------------------------------------//
// GetNextFunction. Returns the next handle for a QA function.  Returns 0 if //
// there are no more handles to expose.                                      //
//---------------------------------------------------------------------------//
XBENCHQA_API DWORD __stdcall GetNextFunction(void)
{
	return (++g_function_index > kFunctionCount) ? 0 : g_function_index;
}

//---------------------------------------------------------------------------//
// SetConfigFile. This function is not currently called by Xbench but must   //
// be exported in the DLL.                                                   //
//---------------------------------------------------------------------------//
XBENCHQA_API void __stdcall SetConfigFile(PSTR aFile)
{
	return;
}

//---------------------------------------------------------------------------//
// ProcessBegin.  Callback called at the beginning of the QA Process.        //
// This is the place to initialize the stuff we are going to need later      //
// during the QA check for the handle. Since this code runs in the UI        //
// thread, a dialog to specify process data may be included there.           //
//---------------------------------------------------------------------------//
XBENCHQA_API void __stdcall ProcessBegin(DWORD aHandle,
										 PQAFunctionParams aParams)
{
  if (aHandle == kCheckLongestHandle)
  {
    for (int i = 0; i < 3; i++)
    {
		g_longest[i].length = 0;
		g_longest[i].segment_id = NULL;
    }
  }
}

//---------------------------------------------------------------------------//
// ProcessEnd. Callback called upon completion of the QA. Here you can free  //
// and unassign anything you previously initialized.                         //
//---------------------------------------------------------------------------//
XBENCHQA_API void __stdcall ProcessEnd(DWORD aHandle)
{
	return;
}

//---------------------------------------------------------------------------//
// ProcessSegment. Called during QA for every segment that has to            //
// be checked, once for each handle have defined with GetFirstFunction and   //
// GetNextFunction.  If it returns a non-NULL PQAPluginResult, the result    //
// will appear in the QA results of the Xbench UI.                           //
//                                                                           //
// For this example, the Suspicious Length Check returns results to Xbench   //
// while the "3 Longest" check does not return anything yet to Xbench        //
// because it has to process all segments before knowing which ones are the  //
// three longest ones.                                                       //
//---------------------------------------------------------------------------//
XBENCHQA_API PQAPluginResult __stdcall ProcessSegment(DWORD aHandle,
													  TQASegmentInfo aSegInfo)
{
	// If no Wide string is available, we'll use the Ansi string
	size_t source_length = (aSegInfo.WSource) ? wcslen(aSegInfo.WSource)
		                                      : strlen(aSegInfo.Source);
	size_t target_length = (aSegInfo.WTarget) ? wcslen(aSegInfo.WTarget)
		                                      : strlen(aSegInfo.Target);

	switch (aHandle)
	{
		case kCheckSuspiciousHandle:
		{
			wchar_t * result_text = CheckSuspiciousLength(source_length,
				                                          target_length);
			if (result_text)
			{
				static TQAPluginResult result;
				result.Text = result_text;
				result.SegId = aSegInfo.SegId;
				result.Options = NULL;
				return &result;
			}
			else
				return NULL;
		}
		case kCheckLongestHandle:
		    return CheckLongest(target_length, aSegInfo.SegId), NULL;
		default:
			return NULL;
	}
}

PQAPluginResult ReadLongestItemsResult(void)
{
	if ((g_longest_index > kLongestCount - 1) ||
		(g_longest[g_longest_index].segment_id == NULL))
		return NULL;

	static TQAPluginResult result;

	const int kBufferSize = 20;
	static wchar_t text_result[kBufferSize];
	result.Text = &text_result[0];
	swprintf(result.Text,
		     kBufferSize - 1,
			 L"Length: %d", g_longest[g_longest_index].length);
	result.SegId = g_longest[g_longest_index].segment_id;

	static TQAPluginResultOptions result_options;
	result.Options = &result_options;
	result.Options->StructVersion = 0;
	result.Options->Sorted = true;
	result.Options->Groupable = true;

	g_longest_index++;

	return &result;
}

//---------------------------------------------------------------------------//
// GetFirstResult. Callback called right after the last call of              //
// ProcessSegment.                                                           //
//                                                                           //
// The first result for the specified handle has to be returned here, or     //
// NULL if no results are to be returned.                                    //
//                                                                           //
// Since this function runs on the QA thread, this is also a good place to   //
// insert a dialog that deals with results before they are actually returned //
// to Xbench (this is how the Xbench spell-checkers work).                   //
//---------------------------------------------------------------------------//
XBENCHQA_API PQAPluginResult __stdcall GetFirstResult(DWORD aHandle)
{
	if (aHandle == kCheckLongestHandle) {
		g_longest_index = 0;
		return ReadLongestItemsResult();
	}

	return NULL;
}

//---------------------------------------------------------------------------//
// GetNextResult.  Xbench will keep calling this funciton for each handle    //
// until NULL is returned to indicate that there are no more results to add  //
// to the Xbench QA results UI.                                              //
//---------------------------------------------------------------------------//
XBENCHQA_API PQAPluginResult __stdcall GetNextResult(DWORD aHandle)
{
	if (aHandle == kCheckLongestHandle)
		return ReadLongestItemsResult();

	return NULL;
}