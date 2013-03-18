//------------------------------------------------------------------------------
// 文件名称：MultiLanguage.h
// 文件版本：v1.0
// 创建日期：2006-02-14 11:25
// 作    者：Richard Ree
// 文件描述：实现多语言切换的头文件
//------------------------------------------------------------------------------

// 语言ID常量枚举，增加、减少语言时直接修改此枚举定义
enum enLANGUAGE
{
	LANGUAGE_GB = 0,
	LANGUAGE_BIG5,
	LANGUAGE_ENGLISH,
	LANGUAGE_BOTTOM
};

// 字符串常量ID枚举，增加、减少表达某个语意的字符串时直接修改此枚举定义
enum enSTRING
{
	STR_NULL = 0,
	STR_AUTO_UPDATE,
	STR_APP_ALREADY_RUNNING,

	STR_PROMPT_NEWER_VERSION_AVAILABLE,
	STR_OPTION_UPGRADE_IN_BACKGROUND,
	STR_PROMPT_UPGRADE_READY,
	STR_PROMPT_FAIL_TO_OPEN_UPDATE_CONFIG_FILE,
	STR_PROMPT_DOWNLOADING_FILE,
	STR_TOTAL_UPGRADE_PROGRESS,
	STR_PROMPT_FAIL_IN_DOWNLOADING_FILES,
	STR_PROMPT_FAIL_IN_VERIFYING_FILES,
	STR_PROMPT_FAIL_IN_UPDATING_FILES,
	STR_PROMPT_FAIL_IN_UPDATING,
	STR_PROMPT_UPGRADE_FINISHED,

	STR_OTHER,

	STR_BUTTON_START_UPGRADE,
	STR_BUTTON_CANCEL_UPGRADE,
	STR_BUTTON_SUCCESS_UPGRADE,

	STR_BUTTON_OK,
	STR_BUTTON_CANCEL,
	STR_BUTTON_ABORT,
	STR_BUTTON_IGANORE,
	STR_BUTTON_RETRY,
	STR_BUTTON_CONTINUE,
	STR_BUTTON_YES,
	STR_BUTTON_NO,
	STR_BUTTON_CLOSE,
	STR_BUTTON_APPLY,

	STR_ERROR,
	STR_ERROR_MESSAGE,

	STRING_BOTTOM
};

// 保存不同语言版本的常量字符串的结构，增加、减少语言时要改写构造函数和Set函数
struct StringStru
{
	const char *Language[LANGUAGE_BOTTOM]; // 保存不同语言版本的常量字符串的指针
	StringStru()
	{
		Language[0] = "";
		Language[1] = "";
		Language[2] = "";
	};
	void Set(const char *s1, const char *s2, const char *s3)
	{
		if (NULL != s1)
		{
			Language[0] = s1;
		}
		if (NULL != s2)
		{
			Language[1] = s2;
		}
		if (NULL != s3)
		{
			Language[2] = s3;
		}
	};
};

// 全局字符串常量表数组（数组初始化在InitStringTable()函数中进行）
extern struct StringStru g_String[STRING_BOTTOM];

// 全局语言代码（注意：随意修改其值可能会导致程序异常！）
extern enum enLANGUAGE g_LanguageID;

// 取得常量字符串的宏定义，STRING_ID为常量字符串ID，PROMPT为任意助记提示字符串，编译时被忽略
#define STRING(STRING_ID, PROMPT) (const char *)(g_String[STRING_ID].Language[g_LanguageID])
