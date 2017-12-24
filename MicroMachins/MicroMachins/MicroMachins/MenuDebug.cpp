#include"stdafx.h"

namespace sft
{
	static	const		int		MENUDEBUG_MAX_LINES = 4096;

	namespace MenuDebug
	{
		Value*		s_pCurrentParent = NULL;
		int			s_iCurrentDirLevel = 0;
		bool		s_bInitialized = false;
		//int			s_iPos = 0;
		Value*		s_pSelectedValue = NULL;
		bool		s_bEnable = true;
		bool		s_bNeedRestart = false;
		float		s_fInputDelay = 0.f;
		float		s_fInputDelayMax = 0.13f;
		Command		s_eLastCommand = INVALID;
		bool		s_bInvertDirection = false;
		char		s_pToSearch[256] = "";

		int s_iSelectedLevel = 0;

		class ValueManager
		{
		public:
			static Value*		GetChildren(const char *pLabel)
			{
				PA_ASSERT(s_pCurrentParent != NULL);
				Value* pValue = s_pCurrentParent->m_pChild;
				while (pValue != NULL)
				{
					if (pValue->m_sLabel == pLabel)
					{
						return pValue;
					}
					pValue = pValue->m_pSiblingNext;
				}

				return NULL;
			}

			static int GetChildCount()
			{
				int iCount = 0;
				Value* pValue = s_pCurrentParent->m_pChild;
				while (pValue != NULL)
				{
					iCount++;
					pValue = pValue->m_pSiblingNext;
				}

				return iCount;
			}

			static Value*		GetNewValue(const char *pLabel, eType eType)
			{
				PA_ASSERT(s_pCurrentParent != NULL);
				PA_ASSERT(s_iNbValue <= MENUDEBUG_MAX_LINES);

				Value* res = &s_oValueList[s_iNbValue];
				res->m_sLabel = pLabel;
				res->m_eType = eType;
				res->SetParent(s_pCurrentParent);
				s_iNbValue++;

				if (s_pSelectedValue == NULL)
				{
					s_pSelectedValue = res;
					s_iSelectedLevel = 1;
				}

				return res;
			}
			static Value*		GetValueList() { return s_oValueList; }
			static int			GetNbValue() { return s_iNbValue; }

			static void			Destroy()
			{
				Value*	pValue = s_oValueList;
				int		iNbValue = s_iNbValue;

				for (int i = 0; i < iNbValue; i++)
				{
					if (COMBO == pValue->m_eType)
					{
						delete pValue->m_pCombo;
						pValue->m_pCombo = NULL;
					}

					pValue->Clear();

					pValue++;
				}

				s_iNbValue = 0;
			}

			static Value*		s_root;
			static Value*		s_oValueList;

		private:
			static int			s_iNbValue;
		};
		Value*		ValueManager::s_oValueList = NULL;
		int			ValueManager::s_iNbValue = 0;
		Value*		ValueManager::s_root = NULL;

		bool			IsInitialized() { return s_bInitialized; }
		bool			IsEnabled() { return s_bEnable; }
		Value*			GetValueList() { return ValueManager::GetValueList(); }
		int				GetNbValue() { return ValueManager::GetNbValue(); }
		Value*			GetSelectedValue() { return s_pSelectedValue; }
		Value*			GetCurrentParentValue() { return s_pCurrentParent; }

		int	Create()
		{
			PA_ASSERT(IsInitialized() == false, "Core::MenuDebug::Create already created");

			s_bEnable = false;
			s_iCurrentDirLevel = 0;
			s_iSelectedLevel = 0;
			s_bInitialized = true;
			ValueManager::s_root = new Value;
			ValueManager::s_oValueList = MENUDEBUG_MAX_LINES > 0 ? new Value[MENUDEBUG_MAX_LINES] : NULL;
			s_pCurrentParent = ValueManager::s_root;
			s_fInputDelay = 0.f;

			return 1;
		}

		void	Destroy()
		{
			Clear();
			PA_SAFE_DELETE_ARRAY(ValueManager::s_oValueList);
			PA_SAFE_DELETE(ValueManager::s_root);
		}

		void	Clear()
		{
			if (IsInitialized())
			{
				ValueManager::Destroy();
				s_bEnable = false;
				s_iCurrentDirLevel = 0;
				s_iSelectedLevel = 0;
				s_pCurrentParent = ValueManager::s_root;
			}
		}

		Combo::Combo()
		{
			m_iNbEntry = 0;
		}

		bool	Combo::AddEntry(std::string sLabel)
		{
			if (m_iNbEntry >= MENUDEBUG_MAX_COMBO_ENTRY)
				return false;

			m_sLabel[m_iNbEntry] = sLabel;
			m_iNbEntry++;
			return true;
		}

		const char*	Combo::GetLabel(int iIdx) const
		{
			if (IsIndexValid(iIdx))
			{
				return m_sLabel[iIdx].c_str();
			}
			else
			{
				return "!<ERROR>!";
			}
		}

		void	SetEnable(bool bEnable)
		{
			s_bEnable = bEnable;
			s_fInputDelay = 0.f;
		}

		bool	GetEnable()
		{
			return s_bEnable;
		}

		bool	GetNeedRestart()
		{
			return s_bNeedRestart;
		}

		void	SetNeedRestart(bool bNeedRestart)
		{
			s_bNeedRestart = bNeedRestart;
		}

		void	SetInvertDirection(bool bInvert)
		{
			s_bInvertDirection = bInvert;
		}

		void	UpdateSearch()
		{
			std::string sLower = ToLower(s_pToSearch);
			Value* pValue = GetValueList();
			while (NULL != pValue)
			{
				pValue->UdpateSearch(sLower.c_str());
				pValue = pValue->m_pSiblingNext;
			}
		}

		char*	GetSearchString()
		{
			return s_pToSearch;
		}

		bool	DirectoryExists(const char* sDirName)
		{
			return ValueManager::GetChildren(sDirName) != NULL;
		}

		int		GetChildCount()
		{
			return ValueManager::GetChildCount();
		}

		bool	OpenDirectory(const char* sDirName, bool bDrawBackGround)
		{
			PA_TODO("find in current parent");

			Value* pValue = ValueManager::GetChildren(sDirName);
			if (pValue == NULL)
			{
				pValue = ValueManager::GetNewValue(sDirName, DIRECTORY);
				if (!pValue) return false;

				pValue->m_pCallback = NULL;
				pValue->m_pCombo = NULL;
				pValue->m_pValue = NULL;
				pValue->m_pUserData = NULL;
				pValue->m_iDirLevel = s_iCurrentDirLevel;
				pValue->m_bFolderOpen = false;
				pValue->m_bDrawBackgroundDirectory = bDrawBackGround;
			}

			s_iCurrentDirLevel++;
			s_pCurrentParent = pValue;
			return true;
		}

		void	CloseDirectory()
		{
			PA_ASSERT(s_iCurrentDirLevel >= 0, PA_T("Base Open/Close directory usage in MenuDebug"));
			s_iCurrentDirLevel--;
			PA_ASSERT(s_pCurrentParent != NULL);
			s_pCurrentParent = s_pCurrentParent->m_pParent;
			PA_ASSERT(s_pCurrentParent != NULL);
		}

		bool	AddBool(std::string sLabel, bool* pStoreValue, ChangedCallback pCallback /* = NULL */, void* pUserData /*= NULL*/, bool _bSaveValue /*= false*/)
		{
			PA_ASSERT(pStoreValue != NULL);

			Value* pValue = ValueManager::GetNewValue(sLabel.c_str(), BOOL);
			if (!pValue) return false;

			pValue->m_pCallback = pCallback;
			pValue->m_pCombo = NULL;
			pValue->m_pValue = (void*)pStoreValue;
			pValue->m_pUserData = pUserData;
			pValue->m_iDirLevel = s_iCurrentDirLevel;
			pValue->m_eDispFormat = FMT_NONE;
			pValue->m_bSave = _bSaveValue;

			return true;
		}

		bool	AddString(std::string  sLabel, std::string* pStoreValue, ChangedCallback pCallback, void* pUserData, bool _bSaveValue)
		{
			PA_ASSERT(pStoreValue != NULL);

			Value* pValue = ValueManager::GetNewValue(sLabel.c_str(), STRING);
			if (!pValue) return false;

			pValue->m_pCallback = pCallback;
			pValue->m_pCombo = NULL;
			pValue->m_pValue = (void*)pStoreValue;
			pValue->m_pUserData = pUserData;
			pValue->m_iDirLevel = s_iCurrentDirLevel;
			pValue->m_eDispFormat = FMT_NONE;
			pValue->m_bSave = _bSaveValue;

			return true;
		}

		//bool	Addcolor(std::string sLabel, sf::Color* pStoreValue, ChangedCallback pCallback /* = NULL */, void* pUserData /*= NULL*/, bool _bSaveValue /*= false*/)
		//{
		//	OpenDirectory("");
		//
		//	AddInt("R", &(pStoreValue->r), 0, 255);
		//
		//	CloseDirectory();
		//}
		//
		//bool	AddChar(std::string sLabel, int* pStoreValue, int iValueMin, int iValueMax, int iStep, ChangedCallback pCallback /* = NULL */, void* pUserData /*= NULL*/, bool bHideIfNull, eDispFormat oIntFormat, bool _bSaveValue /*= false*/)
		//{
		//	PA_ASSERT(iValueMin < iValueMax);
		//	//PA_ASSERT( iValueMin >= 0 );
		//	Value* pValue = ValueManager::GetNewValue(sLabel.c_str(), INT);
		//	if (!pValue) return false;
		//	pValue->m_pCallback = pCallback;
		//	pValue->m_pCombo = NULL;
		//	pValue->m_bHideIfNull = bHideIfNull;
		//	pValue->m_pValue = (void*)pStoreValue;
		//	pValue->m_pUserData = (void*)pUserData;
		//	pValue->m_iValueMin = iValueMin;
		//	pValue->m_iValueMax = iValueMax;
		//	pValue->m_iDirLevel = s_iCurrentDirLevel;
		//	pValue->m_iStep = iStep;
		//	pValue->m_eDispFormat = oIntFormat;
		//	pValue->m_bSave = _bSaveValue;
		//
		//	return true;
		//}

		bool	AddInt(std::string sLabel, int* pStoreValue, int iValueMin, int iValueMax, int iStep, ChangedCallback pCallback /* = NULL */, void* pUserData /*= NULL*/, bool bHideIfNull, eDispFormat oIntFormat, bool _bSaveValue /*= false*/)
		{
			PA_ASSERT(iValueMin < iValueMax);
			//PA_ASSERT( iValueMin >= 0 );
			Value* pValue = ValueManager::GetNewValue(sLabel.c_str(), INT);
			if (!pValue) return false;
			pValue->m_pCallback = pCallback;
			pValue->m_pCombo = NULL;
			pValue->m_bHideIfNull = bHideIfNull;
			pValue->m_pValue = (void*)pStoreValue;
			pValue->m_pUserData = (void*)pUserData;
			pValue->m_iValueMin = iValueMin;
			pValue->m_iValueMax = iValueMax;
			pValue->m_iDirLevel = s_iCurrentDirLevel;
			pValue->m_iStep = iStep;
			pValue->m_eDispFormat = oIntFormat;
			pValue->m_bSave = _bSaveValue;

			return true;
		}

		bool	AddChar(std::string sLabel, char* pStoreValue, char cValueMin, char cValueMax, int iStep, ChangedCallback pCallback /* = NULL */, void* pUserData /*= NULL*/, bool bHideIfNull, bool _bSaveValue /*= false*/)
		{
			PA_ASSERT(cValueMin < cValueMax);
			//PA_ASSERT( iValueMin >= 0 );
			Value* pValue = ValueManager::GetNewValue(sLabel.c_str(), CHAR);
			if (!pValue) return false;
			pValue->m_pCallback = pCallback;
			pValue->m_pCombo = NULL;
			pValue->m_bHideIfNull = bHideIfNull;
			pValue->m_pValue = (void*)pStoreValue;
			pValue->m_pUserData = (void*)pUserData;
			pValue->m_iValueMin = static_cast<int>(cValueMin);
			pValue->m_iValueMax = static_cast<int>(cValueMax);
			pValue->m_iDirLevel = s_iCurrentDirLevel;
			pValue->m_iStep = iStep;
			pValue->m_eDispFormat = FMT_NONE;
			pValue->m_bSave = _bSaveValue;

			return true;
		}

		bool	AddUChar(std::string sLabel, unsigned char* pStoreValue, unsigned char cValueMin, unsigned char cValueMax, int iStep, ChangedCallback pCallback /* = NULL */, void* pUserData /*= NULL*/, bool bHideIfNull, bool _bSaveValue /*= false*/)
		{
			PA_ASSERT(cValueMin < cValueMax);
			//PA_ASSERT( iValueMin >= 0 );
			Value* pValue = ValueManager::GetNewValue(sLabel.c_str(), UCHAR);
			if (!pValue) return false;
			pValue->m_pCallback = pCallback;
			pValue->m_pCombo = NULL;
			pValue->m_bHideIfNull = bHideIfNull;
			pValue->m_pValue = (void*)pStoreValue;
			pValue->m_pUserData = (void*)pUserData;
			pValue->m_iValueMin = static_cast<int>(cValueMin);
			pValue->m_iValueMax = static_cast<int>(cValueMax);
			pValue->m_iDirLevel = s_iCurrentDirLevel;
			pValue->m_iStep = iStep;
			pValue->m_eDispFormat = FMT_NONE;
			pValue->m_bSave = _bSaveValue;

			return true;
		}

		bool	AddFloat(std::string sLabel, float* pStoreValue, float fValueMin, float fValueMax, float fStep, ChangedCallback pCallback, void* pUserData /*= NULL*/, bool bHideIfNull, bool _bSaveValue /*= false*/)
		{
			Value* pValue = ValueManager::GetNewValue(sLabel.c_str(), FLOAT);
			if (!pValue) return false;

			pValue->m_pCallback = pCallback;
			pValue->m_pCombo = NULL;
			pValue->m_bHideIfNull = bHideIfNull;
			pValue->m_pValue = (void*)pStoreValue;
			pValue->m_pUserData = (void*)pUserData;
			pValue->m_fValueMin = fValueMin;
			pValue->m_fValueMax = fValueMax;
			pValue->m_iDirLevel = s_iCurrentDirLevel;
			pValue->m_fStep = fStep;
			pValue->m_eDispFormat = FMT_NONE;
			pValue->m_bSave = _bSaveValue;

			return true;
		}

		bool	AddFunc(std::string sLabel, VoidCallback pStoreValue)
		{
			PA_ASSERT(pStoreValue != NULL);

			Value* pValue = ValueManager::GetNewValue(sLabel.c_str(), FUNC);
			if (!pValue) return false;

			pValue->m_pCallback = NULL;
			pValue->m_pCombo = NULL;
			pValue->m_pValue = (void*)pStoreValue;
			pValue->m_iDirLevel = s_iCurrentDirLevel;
			pValue->m_eDispFormat = FMT_NONE;

			return true;
		}

		bool	AddColor(std::string sLabel, sf::Color* pStoreValue, int fStep, ChangedCallback pCallback, void* pUserData /*= NULL*/, bool bHideIfNull, bool _bSaveValue /*= false*/)
		{
			bool bReturn = true;

			if (OpenDirectory(sLabel.c_str()) == true)
			{
				bReturn &= AddUChar("R", &pStoreValue->r, 0, 255, fStep, pCallback, pUserData, bHideIfNull, _bSaveValue);
				bReturn &= AddUChar("G", &pStoreValue->g, 0, 255, fStep, pCallback, pUserData, bHideIfNull, _bSaveValue);
				bReturn &= AddUChar("B", &pStoreValue->b, 0, 255, fStep, pCallback, pUserData, bHideIfNull, _bSaveValue);
				bReturn &= AddUChar("A", &pStoreValue->a, 0, 255, fStep, pCallback, pUserData, bHideIfNull, _bSaveValue);

				CloseDirectory();
			}
			else
			{
				return false;
			}

			return bReturn;
		}

		bool	AddVector2(std::string sLabel, sf::Vector2f* pStoreValue, float fValueMin, float fValueMax, float fStep, ChangedCallback pCallback, void* pUserData /*= NULL*/, bool bHideIfNull, bool _bSaveValue /*= false*/)
		{
			bool bReturn = true;

			if (OpenDirectory(sLabel.c_str(), false) == true)
			{
				bReturn &= AddFloat("X", &pStoreValue->x, fValueMin, fValueMax, fStep, pCallback, pUserData, bHideIfNull, _bSaveValue);
				bReturn &= AddFloat("Y", &pStoreValue->y, fValueMin, fValueMax, fStep, pCallback, pUserData, bHideIfNull, _bSaveValue);

				CloseDirectory();
			}
			else
			{
				return false;
			}

			return bReturn;
		}

		Combo*	AddCombo(std::string sLabel, int* pStoreValue, ChangedCallback pCallback, void* pUserData /*= NULL*/, bool _bSaveValue /*= false*/)
		{
			PA_ASSERT(pStoreValue != NULL);
			Value* pValue = ValueManager::GetNewValue(sLabel.c_str(), COMBO);
			if (!pValue) return NULL;

			pValue->m_pCallback = pCallback;
			pValue->m_pCombo = new Combo;
			pValue->m_pValue = (void*)pStoreValue;
			pValue->m_pUserData = pUserData;
			pValue->m_iDirLevel = s_iCurrentDirLevel;
			pValue->m_eDispFormat = FMT_NONE;
			pValue->m_bSave = _bSaveValue;

			return pValue->m_pCombo;
		}

		void	Remove(std::string sLabel)
		{
			Value* pValue = ValueManager::GetChildren(sLabel.c_str());

			if (pValue != NULL)
			{
				pValue->Remove();
			}
		}

		Value::Value()
		{
			Clear();
		}

		void Value::Clear()
		{
			m_pValue = NULL;
			/*		m_xValueCallback = NULL;*/
			m_pUserData = NULL;
			m_pCombo = NULL;
			m_eType = UNKNOW;
			m_pCallback = NULL;
			m_iValueMin = 0;
			m_iValueMax = 0;
			m_iDirLevel = 0;
			m_bHideIfNull = false;
			m_bFolderOpen = false;
			m_iStep = 0;
			m_bRendered = false;			// setup each frame, used to navigate in folders without recursion
			m_bSearchFound = true;
			m_pParent = NULL;
			m_pSiblingNext = NULL;
			m_pSiblingPrev = NULL;
			m_pChild = NULL;
			m_bSave = false;
			m_sLabel.clear();
		}

		void Value::Remove()
		{
			if (m_pSiblingPrev == NULL && m_pParent != NULL)
			{
				m_pParent->m_pChild = m_pSiblingNext;
			}
			if (m_pSiblingPrev != NULL)
			{
				m_pSiblingPrev->m_pSiblingNext = m_pSiblingNext;
			}
			if (m_pSiblingNext != NULL)
			{
				m_pSiblingNext->m_pSiblingPrev = m_pSiblingPrev;
			}

			Value* pChild = m_pChild;
			while (pChild != NULL)
			{
				Value* pNextChild = pChild->m_pSiblingNext;
				pChild->Clear();
				pChild = pNextChild;
			}

			Clear();
		}

		void	Value::Format(char* sBuffer, int /*iMax*/)
		{
			char sTmp[32];
			//TODO: Use snprintf and strncat for security
			sprintf_s(sBuffer, sizeof(sBuffer), "%s : ", m_sLabel.c_str());
			switch (m_eType)
			{
			case COMBO:
			{
				PA_ASSERT(m_pCombo, PA_T(""));
				const int iValue = *(int*)m_pValue;
				strcat_s(sBuffer, sizeof(sBuffer), m_pCombo->GetLabel(iValue));
			}
			break;
			case FUNC:
				sprintf_s(sBuffer, sizeof(sBuffer), "%s", m_sLabel.c_str());
				break;
			case BOOL:
				if (*(bool*)m_pValue)
					strcat_s(sBuffer, sizeof(sBuffer), "<ON>");
				else
					strcat_s(sBuffer, sizeof(sBuffer), "<OFF>");
				break;
			case STRING:
				sprintf_s(sBuffer, sizeof(sBuffer), "+ %s", m_sLabel.c_str());
				break;
			case CHAR:
				sprintf_s(sTmp, sizeof(sTmp), "%d", *(char*)m_pValue);
				strcat_s(sBuffer, sizeof(sBuffer), sTmp);
			case UCHAR:
				sprintf_s(sTmp, sizeof(sTmp), "%d", *(unsigned char*)m_pValue);
				strcat_s(sBuffer, sizeof(sBuffer), sTmp);
				break;
			case INT:
			{
				unsigned int iValue = *(int*)m_pValue;

				switch (m_eDispFormat)
				{
				case FMT_BYTE:
				{
					sprintf_s(sTmp, sizeof(sTmp), "%d B", iValue);
					break;
				}
				case FMT_CONVERT_KILOBYTE:
				{
					float fkBytes = ((float)iValue) / 1024.f;
					sprintf_s(sTmp, sizeof(sTmp), "%.3f KB", fkBytes);
					break;
				}
				case FMT_CONVERT_MEGABYTE:
				{
					float fMBytes = ((float)iValue) / (1024.f*1024.f);
					sprintf_s(sTmp, sizeof(sTmp), "%.3f MB", fMBytes);
					break;
				}
				case FMT_NONE:
				{
					sprintf_s(sTmp, sizeof(sTmp), "%d", iValue);
					break;
				}
				default:
				{
					//PA_LOG_WARN("Unknown int format");
					sprintf_s(sTmp, sizeof(sTmp), "%d", iValue);
					break;
				}
				}
				strcat_s(sBuffer, sizeof(sBuffer), sTmp);
			}
			break;
			case FLOAT:
			{
				sprintf_s(sTmp, sizeof(sTmp), "%f", *(float*)m_pValue);
				strcat_s(sBuffer, sizeof(sBuffer), sTmp);
			}
			break;
			case DIRECTORY:
			{
				if (m_bFolderOpen)
				{
					sprintf_s(sBuffer, sizeof(sBuffer), "+ %s", m_sLabel.c_str());
				}
				else
				{
					sprintf_s(sBuffer, sizeof(sBuffer), "[%s]", m_sLabel.c_str());
				}
			}
			break;
			default:
				PA_ASSERT_FALSE();
				break;
			}
		}

		void	Value::Inc(float fIncrementModifier /*= 1.f*/)
		{
			switch (m_eType)
			{
			case COMBO:
			{
				int* pData = (int*)m_pValue;
				*pData += 1;
				if (*pData >= m_pCombo->GetNbEntry())
					*pData = 0;
			}
			break;
			case FUNC:
			{
				((VoidCallback)m_pValue)();
			}
			break;
			case BOOL:
			{
				bool *pData = (bool*)m_pValue;
				*pData ^= true;
			}
			break;
			case CHAR:
			{
				char* pData = (char*)m_pValue;
				*pData += (char)(m_iStep * fIncrementModifier);
				if (*pData > (char)m_iValueMax)
					*pData = static_cast<char>(m_iValueMin);
			}
			break;
			case UCHAR:
			{
				unsigned char* pData = (unsigned char*)m_pValue;
				*pData += (unsigned char)(m_iStep * fIncrementModifier);
				if (*pData > (unsigned char)m_iValueMax)
					*pData = static_cast<unsigned char>(m_iValueMin);
			}
			break;
			case INT:
			{
				int* pData = (int*)m_pValue;
				*pData += (int)(m_iStep * fIncrementModifier);
				if (*pData > m_iValueMax)
					*pData = m_iValueMin;
			}
			break;
			case FLOAT:
			{
				float* pData = (float*)m_pValue;
				*pData += m_fStep * fIncrementModifier;
				if (*pData > m_fValueMax)
					*pData = m_fValueMax;
			}
			break;
			case DIRECTORY:
				m_bFolderOpen = true;
				break;
			default:
				break;
			}
			if (m_pCallback)
			{
				m_pCallback(m_sLabel, m_pValue, m_pUserData);
			}
		}

		void	Value::Enter()
		{
			switch (m_eType)
			{
			case COMBO:
				//case STRINGENUM:
				Inc();
				break;
			case STRING:
			{
				((VoidCallback)m_pValue)();
			}
			case FUNC:
			{
				((VoidCallback)m_pValue)();
			}
			break;
			case BOOL:
			case DIRECTORY:
				m_bFolderOpen ^= true;
				break;
			default:
				break;
			}
			if (m_pCallback)
			{
				m_pCallback(m_sLabel, m_pValue, m_pUserData);
			}
		}

		void	Value::Dec(float fIncrementModifier /*= 1.f*/)
		{
			switch (m_eType)
			{
			case COMBO:
			{
				int* pData = (int*)m_pValue;
				*pData -= 1;
				if (*pData < 0)
					*pData = m_pCombo->GetNbEntry() - 1;
			}
			break;
			case STRING:
			{
				((VoidCallback)m_pValue)();
			}
			case FUNC:
			{
				((VoidCallback)m_pValue)();
			}
			break;
			case BOOL:
				Inc();		// Dec is the same as Inc for bool values
				break;
			case CHAR:
			{
				char* pData = (char*)m_pValue;
				*pData -= (char)(m_iStep * fIncrementModifier);
				if (*pData < m_iValueMin)
					*pData = static_cast<char>(m_iValueMax);
			}
			break;
			case UCHAR:
			{
				unsigned char* pData = (unsigned char*)m_pValue;
				*pData -= (unsigned char)(m_iStep * fIncrementModifier);
				if (*pData < m_iValueMin)
					*pData = static_cast<unsigned char>(m_iValueMax);
			}
			break;
			case INT:
			{
				int* pData = (int*)m_pValue;
				*pData -= (int)(m_iStep * fIncrementModifier);
				if (*pData < m_iValueMin)
					*pData = m_iValueMax;
			}
			break;
			case FLOAT:
			{
				float* pData = (float*)m_pValue;
				*pData -= m_fStep * fIncrementModifier;
				if (*pData < m_fValueMin)
					*pData = m_fValueMin;
			}
			break;
			case DIRECTORY:
				m_bFolderOpen = false;
				break;
			default:
				break;
			}
			if (m_pCallback)
			{
				m_pCallback(m_sLabel, m_pValue, m_pUserData);
			}
		}

		void Value::SetParent(Value* pParent)
		{
			PA_ASSERT(pParent != NULL);
			if (pParent->m_pChild == NULL)
			{
				pParent->m_pChild = this;
			}
			else
			{
				//find last child
				Value* pLastChild = pParent->m_pChild;
				while (pLastChild->m_pSiblingNext != NULL)
				{
					pLastChild = pLastChild->m_pSiblingNext;
				}
				pLastChild->m_pSiblingNext = this;
				m_pSiblingPrev = pLastChild;
			}

			m_pSiblingNext = NULL;
			m_pParent = pParent;
		}

		bool Value::IsSkipped() const
		{
			if (m_bHideIfNull && m_eType == MenuDebug::INT && *((int*)m_pValue) == 0)
				return true;
			if (m_bHideIfNull && m_eType == MenuDebug::FLOAT && *((float*)m_pValue) == 0.f)
				return true;

			return !m_bSearchFound;
		}

		std::string Value::GetPath()const
		{
			Value* pParent = m_pParent;
			std::string sReturn;
			while (pParent)
			{
				if (pParent->m_sLabel != "")
				{
					if (pParent != m_pParent)
						sReturn = "." + sReturn;
					sReturn = pParent->m_sLabel + sReturn;
				}
				pParent = pParent->m_pParent;
			}
			return sReturn;
		}

		bool Value::UdpateSearch(const char* sToSearch)
		{
			bool bRet = false;
			bool bFound = false;
			if (ToLower((const std::string)m_sLabel).find(sToSearch) != std::string::npos)
			{
				bFound = true;
				bRet = true;
			}

			if (m_eType == DIRECTORY)
			{
				Value* pChild = m_pChild;
				while (NULL != pChild)
				{
					bRet |= pChild->UdpateSearch(bFound ? "" : sToSearch); // if found in directory, show all childs
					pChild = pChild->m_pSiblingNext;
				}
			}

			m_bSearchFound = bRet;
			return bRet;
		}

		Value* MoveTo(Value* pValue, int& counter, const int dest)
		{
			if (pValue == NULL) return NULL;
			if (pValue->IsSkipped()) return MoveTo(pValue->m_pSiblingNext, counter, dest);

			counter++;
			if (counter == dest)
			{
				return pValue;
			}

			Value* pRes = NULL;
			if (pValue->m_eType == DIRECTORY && pValue->m_bFolderOpen && pValue->m_pChild != NULL)
			{
				pRes = MoveTo(pValue->m_pChild, counter, dest);
			}
			if (pRes == NULL)
				pRes = MoveTo(pValue->m_pSiblingNext, counter, dest);

			return pRes;
		}

		void	MoveToBegin()
		{
			s_pSelectedValue = ValueManager::s_root->m_pChild;
			s_iSelectedLevel = 0;
		}

		void	MoveToEnd()
		{
			Value* pNextSelected = NULL;
			do
			{
				int counter = 0;
				pNextSelected = MoveTo(ValueManager::s_root->m_pChild, counter, s_iSelectedLevel + 1);
				if (pNextSelected)
				{
					s_pSelectedValue = pNextSelected;
					s_iSelectedLevel++;
				}
			} while (pNextSelected);
		}

		void	MoveUp()
		{
			int counter = 0;
			int increment = s_bInvertDirection ? 1 : -1;
			Value* pNextSelected = MoveTo(ValueManager::s_root->m_pChild, counter, s_iSelectedLevel + increment);
			if (pNextSelected != NULL)
			{
				s_pSelectedValue = pNextSelected;
				s_iSelectedLevel += increment;
			}
			else
			{
				if (s_bInvertDirection)
					MoveToBegin();
				else
					MoveToEnd();
			}
		}

		void	MoveDown()
		{
			int counter = 0;
			int increment = s_bInvertDirection ? -1 : 1;
			Value* pNextSelected = MoveTo(ValueManager::s_root->m_pChild, counter, s_iSelectedLevel + increment);
			if (pNextSelected != NULL)
			{
				s_pSelectedValue = pNextSelected;
				s_iSelectedLevel += increment;
			}
			else
			{
				if (s_bInvertDirection)
					MoveToEnd();
				else
					MoveToBegin();
			}
		}

		void Update(float fDeltaTime)
		{
			if (GetEnable())
				s_fInputDelay += fDeltaTime;
		}

		bool	SendCommand(const Command eCommand, bool bImmediate)
		{
			bool bOk = false;

			if (s_bEnable)
			{
				Value* pValue = GetSelectedValue();

				if (eCommand == ENTER)
				{
					pValue->Enter();
				}

				if (s_fInputDelay >= s_fInputDelayMax || eCommand != s_eLastCommand || bImmediate)
				{
					switch (eCommand)
					{
					case UP:				MoveUp(); break;
					case DOWN:				MoveDown(); break;
					case INC_VALUE:			pValue->Inc(); break;
					case DEC_VALUE:			pValue->Dec(); break;
					case INCREMENTx10:		pValue->Inc(10.f); break;
					case INCREMENTx01:		pValue->Inc(0.1f); break;
					case DECREMENTx10:		pValue->Dec(10.f); break;
					case DECREMENTx01:		pValue->Dec(0.1f); break;
					default: break;
					}
					s_eLastCommand = eCommand;
					s_fInputDelay = 0.f;
				}

				bOk = true;
			}
			return bOk;
		}

		Value* GetRootValue()
		{
			return ValueManager::s_root;
		}

		void GetSelectedLevel(Value* pValue, int &i)
		{
			if (pValue != NULL)return;
			i++;

			GetSelectedLevel(pValue->m_pSiblingNext, i);
			if (pValue->m_eType == DIRECTORY && pValue->m_bFolderOpen)
				GetSelectedLevel(pValue->m_pChild, i);
		}
		int GetSelectedLevel()
		{
			return s_iSelectedLevel;
		}
	}

	MenuDebugBool::MenuDebugBool(const char* sName, bool bValue)
	{
		m_bValue = bValue;
		MenuDebug::AddBool(sName, &m_bValue);
	}

	MenuDebugFloat::MenuDebugFloat(const char* sName, float fValue, float fMin, float fMax, float fStep)
	{
		m_fValue = fValue;
		MenuDebug::AddFloat(sName, &m_fValue, fMin, fMax, fStep);
	}

	MenuDebugInt::MenuDebugInt(const char* sName, int iValue, int iMin, int iMax)
	{
		m_iValue = iValue;
		MenuDebug::AddInt(sName, &m_iValue, iMin, iMax);
	}
}