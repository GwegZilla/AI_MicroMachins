#ifndef __PLAYALL_CORE_MENUDEBUG_H__
#define __PLAYALL_CORE_MENUDEBUG_H__

#include "stdafx.h"

namespace sft
{
	static	const		int		MENUDEBUG_MAX_COMBO_ENTRY = 32;

	struct StatSlot;
	class StringEnumeration;

	namespace MenuDebug			// MenuDebug is not a class to avoid a singleton
	{
		typedef	void(*ChangedCallback)(const std::string& sLabel, void* pValue, void* pUserData);
		typedef	void(*VoidCallback)();
		typedef	void(*DataCallback)(void*);
		class	Value;

		enum Command
		{
			UP,
			DOWN,
			INC_VALUE,
			DEC_VALUE,
			ENTER,
			INCREMENTx10,
			INCREMENTx01,
			DECREMENTx10,
			DECREMENTx01,

			INVALID,
		};

		enum eDispFormat
		{
			FMT_NONE,
			FMT_BYTE,
			FMT_CONVERT_KILOBYTE,
			FMT_CONVERT_MEGABYTE
		};

		class Combo
		{
		public:
			Combo();
			bool			AddEntry(std::string  sLabel);
			bool			IsIndexValid(int iIdx)	const { return ((iIdx >= 0) && (iIdx < m_iNbEntry)); }
			const char* 	GetLabel(int iIdx) const;
			int				GetNbEntry()	const { return m_iNbEntry; }

		private:

			int					m_iNbEntry;
			std::string 		m_sLabel[MENUDEBUG_MAX_COMBO_ENTRY];
		};
		//-----------------------------------------------------------------
		// Public func
		//-----------------------------------------------------------------
		int				Create();
		void			Destroy();

		void			Update(float fDeltaTime);

		void			Clear();
		bool			IsInitialized();
		bool			IsEnabled();
		Value*			GetValueList();
		Value*			GetRootValue();
		Value*			GetSelectedValue();
		Value*			GetCurrentParentValue();
		int				GetSelectedLevel();
		int				GetNbValue();
		bool			DirectoryExists(const char* sDirName);
		int				GetChildCount();

		bool			OpenDirectory(const char*, bool bDrawBackGround = true);
		void			CloseDirectory();

		bool			AddBool(std::string  sLabel, bool* pStoreValue, ChangedCallback pCallback = NULL, void* pUserData = NULL, bool _bSaveValue = false);
		bool			AddInt(std::string  sLabel, int* pStoreValue, int iValueMin, int iValueMax, int iStep = 1, ChangedCallback pCallback = NULL, void* pUserData = NULL, bool bHideIfNull = false, eDispFormat oIntFormat = FMT_NONE, bool _bSaveValue = false);
		bool			AddChar(std::string sLabel, char* pStoreValue, char cValueMin = -128, char cValueMax = 127, int iStep = 1.0f, ChangedCallback pCallback = NULL, void* pUserData = NULL, bool bHideIfNull = false, bool _bSaveValue = false);
		bool			AddFloat(std::string  sLabel, float* pStoreValue, float fValueMin, float fValueMax, float fStep = 1.f, ChangedCallback pCallback = NULL, void* pUserData = NULL, bool bHideIfNull = false, bool _bSaveValue = false);
		bool			AddFunc(std::string sLabel, VoidCallback pStoreValue);
		bool			AddColor(std::string sLabel, sf::Color* pStoreValue, int fStep = 1, ChangedCallback pCallback = NULL, void* pUserData = NULL, bool bHideIfNull = false, bool _bSaveValue = false);
		bool			AddUChar(std::string sLabel, unsigned char* pStoreValue, unsigned char cValueMin = 0, unsigned char cValueMax = 255, int iStep = 1, ChangedCallback pCallback = NULL, void* pUserData = NULL, bool bHideIfNull = false, bool _bSaveValue = false);
		bool			AddVector2(std::string sLabel, sf::Vector2f* pStoreValue, float fValueMin, float fValueMax, float fStep = 1.0f, ChangedCallback pCallback = NULL, void* pUserData = NULL, bool bHideIfNull = false, bool _bSaveValue = false);
		Combo*			AddCombo(std::string sLabel, int* pStoreValue, ChangedCallback pCallback = NULL, void* pUserData = NULL, bool _bSaveValue = false);
		bool			AddString(std::string  sLabel, std::string* pStoreValue, ChangedCallback pCallback = NULL, void* pUserData = NULL, bool _bSaveValue = false);

		void			Remove(std::string  sLabel);

		bool			SendCommand(const Command eCommand, bool bImmediate);

		void			SetEnable(bool bEnable);
		bool			GetEnable();

		void			SetNeedRestart(bool bNeedRestart);
		bool			GetNeedRestart();

		void			SetInvertDirection(bool bInvertDirection);

		void			UpdateSearch();
		char*			GetSearchString();

		//-----------------------------------------------------------------
		// Private usage
		//-----------------------------------------------------------------s

		enum eType
		{
			UNKNOW,
			BOOL,
			INT,
			CHAR,
			UCHAR,
			FLOAT,
			STRING,
			FUNC,
			COMBO,
			DIRECTORY,
		};

		class Value
		{
		public:

			Value();

			void	Clear();
			void	Remove();

			void	Format(char* sBuffer, int iMax);
			void	Inc(float fIncrementModifier = 1.f);
			void	Dec(float fIncrementModifier = 1.f);
			void	Enter();

			void	SetParent(Value* pParent);
			bool	IsSkipped() const;
			std::string	GetPath()const;

			bool	UdpateSearch(const char* pToSearch);

			std::string			m_sLabel;
			void*				m_pValue;
			//		CallbackHandle		m_xValueCallback;
			void*				m_pUserData;
			Combo*				m_pCombo;
			eType				m_eType;
			eDispFormat			m_eDispFormat;
			ChangedCallback		m_pCallback;
			int					m_iValueMin;
			int					m_iValueMax;
			int					m_iStep;
			float				m_fValueMin;
			float				m_fValueMax;
			float				m_fStep;
			int					m_iDirLevel;
			bool				m_bHideIfNull;
			bool				m_bFolderOpen;
			bool				m_bRendered;			// setup each frame, used to navigate in folders without recursion
			bool				m_bSearchFound;
			bool				m_bSave;
			bool				m_bDrawBackgroundDirectory;
			Value*				m_pParent;
			Value*				m_pSiblingNext;
			Value*				m_pSiblingPrev;
			Value*				m_pChild;

			// Used for StatCounter
			std::string 		m_sLabelUnity;
			int					m_iDivFactor;			// div factor to display stats
		};
	};

	inline std::string& ToUpper(std::string& str)
	{
		transform(str.begin(), str.end(), str.begin(), (int(*)(int))toupper);
		return str;
	}

	inline std::string ToUpper(const std::string& str)
	{
		std::string strRes(str);
		transform(str.begin(), str.end(), strRes.begin(), (int(*)(int))toupper);
		return strRes;
	}

	inline std::string& ToLower(std::string& str)
	{
		transform(str.begin(), str.end(), str.begin(), (int(*)(int))tolower);
		return str;
	}

	inline std::string ToLower(const std::string& str)
	{
		std::string strRes(str);
		transform(str.begin(), str.end(), strRes.begin(), (int(*)(int))tolower);
		return strRes;
	}

	// Can be added as a static local variable to debug using MenuDebug
	struct MenuDebugBool
	{
		MenuDebugBool(const char* sName, bool bValue);

		operator bool() const
		{
			return m_bValue;
		}

		bool m_bValue;
	};

	struct MenuDebugFloat
	{
		MenuDebugFloat(const char* sName, float fValue, float fMin, float fMax, float fStep);

		operator float() const
		{
			return m_fValue;
		}

		float m_fValue;
	};

	struct MenuDebugInt
	{
		MenuDebugInt(const char* sName, int iValue, int iMin, int iMax);

		operator int() const
		{
			return m_iValue;
		}

		int m_iValue;
	};
}

#endif //__PLAYALL_CORE_MENUDEBUG_H__
