/* --------------------------------------------------------------------------------
 #
 #	4DPlugin.c
 #	source generated by 4D Plugin Wizard
 #	Project : Source Highlight
 #	author : miyako
 #	2013/08/03
 #
 # --------------------------------------------------------------------------------*/


#include "4DPluginAPI.h"
#include "4DPlugin.h"

#include <fstream>
#include <iostream>
#include <sstream>

#include <srchilite/sourcehighlight.h>
#include <srchilite/langmap.h>

using namespace std;	
using namespace srchilite;

#define THIS_BUNDLE_ID @"com.4D.4DPlugin.miyako.SourceHighlight"
#define THIS_BUNDLE_NAME L"Source Highlight.4DX"

C_TEXT langFolderPath;

void PluginMain(int32_t selector, PA_PluginParameters params)
{
	try
	{
		int32_t pProcNum = selector;
		sLONG_PTR *pResult = (sLONG_PTR *)params->fResult;
		PackagePtr pParams = (PackagePtr)params->fParameters;

		CommandDispatcher(pProcNum, pResult, pParams); 
	}
	catch(...)
	{

	}
}

void CommandDispatcher (int32_t pProcNum, sLONG_PTR *pResult, PackagePtr pParams)
{
	switch(pProcNum)
	{
		case kInitPlugin :
		case kServerInitPlugin :			
			_getDefaultLangFolderPath(&langFolderPath);
			break;					
// --- source-highlight

		case 1 :
			CODE_HIGHLIGHT(pResult, pParams);
			break;

// --- preferences

		case 2 :
			CODE_GET_LANGUAGE_LIST(pResult, pParams);
			break;

		case 3 :
			CODE_Set_language_folder(pResult, pParams);
			break;

		case 4 :
			CODE_Get_language_folder(pResult, pParams);
			break;

	}
}

void _getLangFolderPath(C_TEXT *p){
#if VERSIONMAC	
	CUTF8String path;
	langFolderPath.copyUTF8String(&path);
	
	NSString *str = (NSString *)CFStringCreateWithFileSystemRepresentation(kCFAllocatorDefault, (const char *)path.c_str());
	
	if(str){
		
		NSURL *u = (NSURL *)CFURLCreateWithFileSystemPath(kCFAllocatorDefault, (CFStringRef)str, kCFURLPOSIXPathStyle, true);
		
		if(u){
			
			NSString *path = (NSString *)CFURLCopyFileSystemPath((CFURLRef)u, kCFURLHFSPathStyle);			
			p->setUTF16String(path);
			[path release];			
			[u release];		
		}
		
		[str release];	
	}
#else
	p->setUTF16String(langFolderPath.getUTF16StringPtr(), langFolderPath.getUTF16Length());
#endif
}

#if VERSIONMAC
void _copyFilePath(C_TEXT *t, CUTF8String *p){
	
	NSString *str = t->copyUTF16String();
	NSURL *u = (NSURL *)CFURLCreateWithFileSystemPath(kCFAllocatorDefault, (CFStringRef)str, kCFURLHFSPathStyle, false);
	
	if(u){
		NSString *path = (NSString *)CFURLCopyFileSystemPath((CFURLRef)u, kCFURLPOSIXPathStyle);
		CFIndex size = CFStringGetMaximumSizeOfFileSystemRepresentation((CFStringRef)path);
		std::vector<uint8_t> buf(size);		
		[path getFileSystemRepresentation:(char *)&buf[0] maxLength:size];
		*p = CUTF8String((const uint8_t *)&buf[0]);
		[path release];
		[u release];
	}	
	
	[str release];
	
}
#else
void _copyFilePath(C_TEXT *t, CUTF16String *p){
	
	*p = CUTF16String((PA_Unichar *)t->getUTF16StringPtr());
	if(p->at(p->size() - 1) == L'\\')
		*p = p->substr(0, p->size() - 1);
	
}
#endif

#if VERSIONMAC
bool _getDefaultLangFolderPath(CUTF8String *p){
	
	bool success = false;
	
	NSBundle *thisBundle = [NSBundle bundleWithIdentifier:THIS_BUNDLE_ID];
	if(thisBundle){
		NSString *path = [[thisBundle resourcePath]stringByAppendingPathComponent:@"source-highlight"];	
		if(path){
			CFIndex size = CFStringGetMaximumSizeOfFileSystemRepresentation((CFStringRef)path);
			vector<uint8_t> buf(size);	
			[path getFileSystemRepresentation:(char *)&buf[0] maxLength:size];
			*p = CUTF8String((const uint8_t *)&buf[0]);
			success = true;
		}
	}
	
	return success;
	
}
#else
bool _getDefaultLangFolderPath(CUTF16String *p){
	
	bool success = false;
	
	wchar_t	fDrive[_MAX_DRIVE],
	fDir[_MAX_DIR],
	fName[_MAX_FNAME],
	fExt[_MAX_EXT];
	
	wchar_t thisPath[_MAX_PATH] = {0};
	wchar_t resourcesPath[_MAX_PATH] = {0};
	wchar_t srchilitePath[_MAX_PATH] = {0};
	
	HMODULE hplugin = GetModuleHandleW(THIS_BUNDLE_NAME);
	GetModuleFileNameW(hplugin, thisPath, _MAX_PATH);
	
	_wsplitpath_s(thisPath, fDrive, fDir, fName, fExt);	
	
	wstring windowsPath = fDrive;
	windowsPath += fDir;
	if(windowsPath.at(windowsPath.size() - 1) == L'\\')
		windowsPath = windowsPath.substr(0, windowsPath.size() - 1);
	
	_wsplitpath_s(windowsPath.c_str(), fDrive, fDir, fName, fExt);
	_wmakepath_s(resourcesPath, fDrive, fDir, L"Resources\\", NULL);
	
	_wsplitpath_s(resourcesPath, fDrive, fDir, fName, fExt);
	_wmakepath_s(srchilitePath, fDrive, fDir, L"source-highlight", NULL);
	
	*p = CUTF16String((PA_Unichar *)srchilitePath);	
	success = true;
	
	return success;
	
}
#endif

void _setDefaultLangFolderPath(C_TEXT *p){
#if VERSIONMAC
	CUTF8String path;	
	_copyFilePath(p, &path);
	langFolderPath.setUTF8String(&path);
#else
	CUTF16String path;	
	_copyFilePath(p, &path);
	langFolderPath.setUTF16String(&path);	
#endif	
}

void _getDefaultLangFolderPath(C_TEXT *p){
#if VERSIONMAC
	CUTF8String path;
	if(_getDefaultLangFolderPath(&path))
		p->setUTF8String(&path);			
#else	
	CUTF16String path;	
	if(_getDefaultLangFolderPath(&path))
		p->setUTF16String(&path);
#endif
}

void _getInLang(C_TEXT *l, string *inLang){
	
	*inLang = "4d.lang";
	
	if(l->getUTF16Length()){
		
		CUTF8String lang;
		l->copyUTF8String(&lang);
		
		size_t found = lang.find((const uint8_t *)".");
		
		if (found != CUTF8String::npos) lang = lang.substr(0, found);
		
		lang += (const uint8_t *)".lang";
		*inLang = (const char *)lang.c_str();
	}
}

void _getOutLang(C_TEXT *l, string *outLang){
	
	*outLang = "html.outlang";
	
	if(l->getUTF16Length()){
		
		CUTF8String lang;
		l->copyUTF8String(&lang);
		
		size_t found = lang.find((const uint8_t *)".");
		
		if (found != CUTF8String::npos) lang = lang.substr(0, found);
		
		lang += (const uint8_t *)".outlang";		
		*outLang = (const char *)lang.c_str();
	}
}

void _getDirPath(string *dirPath){
#if VERSIONMAC	
	CUTF8String p;
	langFolderPath.copyUTF8String(&p);
	*dirPath = (const char *)p.c_str();
#else
	setlocale(LC_CTYPE, "");	
	CUTF16String p;	
	langFolderPath.copyUTF16String(&p);	
	size_t size = (p.length() * MB_CUR_MAX) + 1;	
	vector<char> buf(size);
	
	/*	TODO: add wstring api to srchilite?
	 http://lists.gnu.org/archive/html/help-source-highlight/2010-03/msg00010.html
	 */
	
	wcstombs((char *)&buf[0], (const wchar_t *)p.c_str(), size);
	*dirPath = (const char *)&buf[0];
#endif	
}

size_t _getLangNames(ARRAY_TEXT *n){
	
	string dirPath;
	
	_getDirPath(&dirPath);
	
	LangMap langMap(dirPath, "lang.map");
	
	langMap.open();
	
	set<string> langNames = langMap.getLangNames();
	
	set<string>::iterator i = langNames.begin();
	
	size_t size = langNames.size();
	
	n->setSize(0);	
	
	if(size){
		
		n->setSize(1);
		
		while(i != langNames.end())
		{
			string langName = (*i);
			n->appendUTF8String((const uint8_t *)langName.c_str(), langName.size());
			++i;
		}	
	} 
	
	return size;
	
}

void _initHighlighter(SourceHighlight *s){
	
	string dirPath;
	
	_getDirPath(&dirPath);
	
	s->setDataDir(dirPath);
}

// ------------------------------- source-highlight -------------------------------


void CODE_HIGHLIGHT(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_TEXT Param2;
	C_TEXT Param3;
	C_TEXT Param4;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	Param2.fromParamAtIndex(pParams, 2);
	Param3.fromParamAtIndex(pParams, 3);
	Param4.fromParamAtIndex(pParams, 4);
	
	string inLang;
	string outLang;
	
	_getInLang(&Param3, &inLang);
	_getOutLang(&Param4, &outLang);	
	
	SourceHighlight sourceHighlight(outLang);
	
	_initHighlighter(&sourceHighlight);
	
	CUTF8String inText;
	Param1.copyUTF8String(&inText);
	
	istringstream iss((const char *)inText.c_str());
	ostringstream oss;
	
	sourceHighlight.highlight(iss, oss, inLang);
	
	string outText = oss.str();
	
	Param2.setUTF8String((const uint8_t *)outText.c_str(), outText.size());
	Param2.toParamAtIndex(pParams, 2);
	
}


// ---------------------------------- preferences ---------------------------------


void CODE_GET_LANGUAGE_LIST(sLONG_PTR *pResult, PackagePtr pParams)
{
	ARRAY_TEXT Param1;
	
	_getLangNames(&Param1);	
	
	Param1.toParamAtIndex(pParams, 1);
}

void CODE_Set_language_folder(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT Param1;
	C_LONGINT returnValue;
	
	Param1.fromParamAtIndex(pParams, 1);
	
	if(Param1.getUTF16Length()){
		
		_setDefaultLangFolderPath(&Param1);	
		
		ARRAY_TEXT names;
		
		try{
			if(_getLangNames(&names)) returnValue.setIntValue(1);
		}catch(...){
			returnValue.setIntValue(0);
			_getDefaultLangFolderPath(&langFolderPath);			
		}
		
	}else{
		_getDefaultLangFolderPath(&langFolderPath);	
		returnValue.setIntValue(1);			
	}
	
	returnValue.setReturn(pResult);
}

void CODE_Get_language_folder(sLONG_PTR *pResult, PackagePtr pParams)
{
	C_TEXT returnValue;
	
	_getLangFolderPath(&returnValue);
	
	returnValue.setReturn(pResult);
}
