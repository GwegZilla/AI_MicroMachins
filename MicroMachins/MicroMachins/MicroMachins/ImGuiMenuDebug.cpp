#include"stdafx.h"
#include "imgui.h"
#include "imgui_internal.h"

namespace sft
{
	static const int s_iSLiderMaxRange = 10;
	static const float s_fIndentWidth = 30.f;
	static const float s_fStartSpaceWidth = 5.f;

	void ImGuiMenuDebugValue(MenuDebug::Value* pValue, bool bOpenDefault)
	{
		if (pValue == NULL)
			return;

		if (!pValue->IsSkipped())
		{
			bool bCallback = false;

			if (pValue->m_eType == MenuDebug::DIRECTORY)
			{
				const char* pLabel = pValue->m_sLabel.c_str();
				if (pValue->m_iDirLevel == 0)
				{
					ImGui::Columns(1);
					ImGui::PushID(pValue);
					bool bOpen = ImGui::CollapsingHeader(pLabel, NULL, pValue->m_bDrawBackgroundDirectory, bOpenDefault);
					ImGui::PopID();
					ImGui::Columns(2);
					if (bOpen)
					{
						ImGuiMenuDebugValue(pValue->m_pChild, bOpenDefault);
					}
				}
				else
				{
					if (bOpenDefault)
					{
						ImGui::SetNextTreeNodeOpen(true, ImGuiSetCond_Appearing);
					}

					const float fCollapserWidth = ImGui::GetWindowFontSize() + ImGui::GetStyle().FramePadding.x * 3.f;
					const float fSpace = MATH::Max(0.f, pValue->m_iDirLevel * s_fIndentWidth - fCollapserWidth + s_fStartSpaceWidth);
					ImGui::Dummy(ImVec2(fSpace, 0.f));
					ImGui::SameLine(0.f, 0.f);
					ImGui::PushID(pValue);
					bool bOpen = ImGui::CollapsingHeader(pLabel, NULL, false, bOpenDefault);
					ImGui::PopID();
					ImGui::NextColumn();
					ImGui::NextColumn();
					if (bOpen)
					{
						ImGuiMenuDebugValue(pValue->m_pChild, bOpenDefault);
					}
				}
			}
			else
			{
				const char* pRealLabel = pValue->m_sLabel.c_str();
				const char* const pLabel = "##Dummy";

				if (pValue->m_eType == MenuDebug::FUNC)
				{
					ImGui::Columns(1);
					ImGui::Dummy(ImVec2(pValue->m_iDirLevel * s_fIndentWidth + s_fStartSpaceWidth, 0.f));
					ImGui::SameLine(0.f, 0.f);
					if (ImGui::Button(pRealLabel, ImVec2(-1.f, 0.f)))
					{
						((MenuDebug::VoidCallback)pValue->m_pValue)();
						bCallback = true;
					}
					ImGui::Columns(2);
				}
				else
				{
					ImGui::Dummy(ImVec2(pValue->m_iDirLevel * s_fIndentWidth + s_fStartSpaceWidth, 0.f));
					ImGui::SameLine(0.f, 0.f);
					ImGui::TextUnformatted(pRealLabel);
					ImGui::NextColumn();
					ImGui::PushItemWidth(-1);
					ImGui::PushID(pValue);

					char* sTemp = NULL;
					const char* sTepmCst = NULL;
					std::string* pString = NULL;

					switch (pValue->m_eType)
					{
					case MenuDebug::STRING:
						pString = ((std::string*)pValue->m_pValue);
						sTepmCst = pString->c_str();
						sTemp = const_cast<char*>(sTepmCst);
						if (ImGui::InputText(pLabel, sTemp, 255))
						{
							bCallback = true;
						}
						break;
					case MenuDebug::BOOL:
					{
						if (ImGui::Checkbox(pLabel, (bool*)pValue->m_pValue))
							bCallback = true;
						break;
					}
					case MenuDebug::FLOAT:
					{
						if (pValue->m_fStep == 0.0f)
						{
							char sFormat[256];
							pValue->Format(sFormat, sizeof(sFormat));
							ImGui::Text(sFormat);
						}
						else
						{
							if (pValue->m_fValueMax - pValue->m_fValueMin <= s_iSLiderMaxRange)
							{
								if (ImGui::SliderFloat(pLabel, (float*)pValue->m_pValue, pValue->m_fValueMin, pValue->m_fValueMax))
									bCallback = true;
							}
							else
							{
								if (ImGui::InputFloat(pLabel, (float*)pValue->m_pValue, pValue->m_fStep, pValue->m_fStep * 10.0f))
								{
									float* pData = (float*)pValue->m_pValue;
									*pData = (float)MATH::Min(*pData, pValue->m_fValueMax);
									*pData = (float)MATH::Max(*pData, pValue->m_fValueMin);
									bCallback = true;
								}
							}
						}
						break;
					}
					case MenuDebug::CHAR:
						if (pValue->m_iStep <= 0)
						{
							char sFormat[256];
							pValue->Format(sFormat, sizeof(sFormat));
							ImGui::Text(sFormat);
						}
						else
						{
							if (pValue->m_iValueMax - pValue->m_iValueMin <= s_iSLiderMaxRange)
							{
								if (ImGui::SliderChar(pLabel, (char*)pValue->m_pValue, pValue->m_iValueMin, pValue->m_iValueMax))
									bCallback = true;
							}
							else
							{
								if (ImGui::InputChar(pLabel, (char*)pValue->m_pValue, pValue->m_iStep, pValue->m_iStep * 10))
								{
									char* pData = (char*)pValue->m_pValue;
									*pData = (char)MATH::Min(*pData, (char)pValue->m_iValueMax);
									*pData = (char)MATH::Max(*pData, (char)pValue->m_iValueMin);
									bCallback = true;
								}
							}
						}
						break;
					case MenuDebug::UCHAR:
						if (pValue->m_iStep <= 0)
						{
							char sFormat[256];
							pValue->Format(sFormat, sizeof(sFormat));
							ImGui::Text(sFormat);
						}
						else
						{
							if (pValue->m_iValueMax - pValue->m_iValueMin <= s_iSLiderMaxRange)
							{
								if (ImGui::SliderUchar(pLabel, (unsigned char*)pValue->m_pValue, pValue->m_iValueMin, pValue->m_iValueMax))
									bCallback = true;
							}
							else
							{
								if (ImGui::InputUChar(pLabel, (unsigned char*)pValue->m_pValue, pValue->m_iStep, pValue->m_iStep * 10))
								{
									unsigned char* pData = (unsigned char*)pValue->m_pValue;
									*pData = (unsigned char)MATH::Min(*pData, (unsigned char)pValue->m_iValueMax);
									*pData = (unsigned char)MATH::Max(*pData, (unsigned char)pValue->m_iValueMin);
									bCallback = true;
								}
							}
						}
						break;
					case MenuDebug::INT:
					{
						if (pValue->m_iStep <= 0)
						{
							char sFormat[256];
							pValue->Format(sFormat, sizeof(sFormat));
							ImGui::Text(sFormat);
						}
						else
						{
							if (pValue->m_iValueMax - pValue->m_iValueMin <= s_iSLiderMaxRange)
							{
								if (ImGui::SliderInt(pLabel, (int*)pValue->m_pValue, pValue->m_iValueMin, pValue->m_iValueMax))
									bCallback = true;
							}
							else
							{
								if (ImGui::InputInt(pLabel, (int*)pValue->m_pValue, pValue->m_iStep, pValue->m_iStep * 10))
								{
									int* pData = (int*)pValue->m_pValue;
									*pData = (int)MATH::Min(*pData, pValue->m_iValueMax);
									*pData = (int)MATH::Min(*pData, pValue->m_iValueMin);
									bCallback = true;
								}
							}
						}
						break;
					}
					case MenuDebug::COMBO:
					{
						char* pLabels[MENUDEBUG_MAX_COMBO_ENTRY];
						for (int i = 0; i < pValue->m_pCombo->GetNbEntry(); i++)
							pLabels[i] = (char*)pValue->m_pCombo->GetLabel(i);
						if (ImGui::Combo(pLabel, (int*)pValue->m_pValue, (const char**)pLabels, pValue->m_pCombo->GetNbEntry()))
							bCallback = true;
						break;
					}
					default:
						ImGui::TextColored(ImVec4(1, 0.5f, 0.f, 1.f), "(unknown type)");
						break;
					}
					ImGui::PopID();
					ImGui::PopItemWidth();

					ImGui::NextColumn();
				}
			}

			if (bCallback && pValue->m_pCallback != NULL)
			{
				pValue->m_pCallback(pValue->m_sLabel, pValue->m_pValue, pValue->m_pUserData);
			}
		}

		ImGuiMenuDebugValue(pValue->m_pSiblingNext, bOpenDefault);
	}

	/*
	static	const	float		MENU_ENTRY_HIGH = (16.f / 600.f);
	float RenderSubGroup(Core::Budget::Group *pGroup, float x, float y)
	{
		for (int iSub = 0;iSub < PA_BUDGET_MAX_SUBGROUP;iSub++)
		{
			Core::Budget::SubGroup *pSubGroup = &pGroup->oSubGroups[iSub];
			float fPosXSub = x + 0.1f;
			if (pSubGroup->bActive && pSubGroup->iCount > 0)
			{
				const float fSubSize = (float)pSubGroup->iCount / (1024.0f*1024.0f);
				GetRenderUtils()->Printf2D(fPosXSub, y, Graphics::O_WHITE, "Sub %d %s %.2f", iSub, pSubGroup->sName.c_str(), fSubSize);
				y -= MENU_ENTRY_HIGH * 2;
			}
		}
		return y;
	}*/

	void ImGuiRenderMenuDebugRoot()
	{
		MenuDebug::Value* root = MenuDebug::GetValueList();
		// Right align, keep 150 pixels for labels

		ImGuiWindow* pWindow = ImGui::GetCurrentWindow();
		if (!pWindow->WasActive)
			ImGui::SetKeyboardFocusHere(); // Auto focus previous widget

		if (ImGui::InputText("Search", MenuDebug::GetSearchString(), 255))
		{
			MenuDebug::UpdateSearch();
		}
		ImGui::PushItemWidth(-150);
		bool bHasSearchText = MenuDebug::GetSearchString()[0] != '\0';
		if (bHasSearchText)
		{
			ImGui::PushID("SearchText");
		}
		ImGui::Columns(2);
		ImGuiMenuDebugValue(root, bHasSearchText);
		ImGui::Columns(1);
		if (bHasSearchText)
		{
			ImGui::PopID();
		}
	}

	void ImGuiRenderMenuDebug()
	{
		if (MenuDebug::IsEnabled())
		{
			ImGui::SetNextWindowPos(ImVec2(50, 30), ImGuiSetCond_FirstUseEver);
			ImGui::SetNextWindowSize(ImVec2(350, 500), ImGuiSetCond_Appearing);
			if (ImGui::Begin("MenuDebug", NULL))
			{
				ImGuiRenderMenuDebugRoot();
			}
			ImGui::End();
		}
	}

	/*void ImGuiRenderBudgets()
	{
		PA_TODO("changer pour faire une fenetre imgui")
			float fPosX = 0.25f;
		if (Core::Budget::GetCombo() != 0)
		{
			float y = 0.9f;
			static StringId sNullGroup = "Null Group";
			for (int i = 0;i < Core::Budget::GetGroupCount();i++)
			{
				Core::Budget::Group* pGroup = Core::Budget::GetGroup(i);
				if (pGroup->sName == sNullGroup)
					continue;

				{
					float fSize = 0.f;
					float fMaxSize = 0.f;
					if (pGroup->bMemory)
					{
						fSize = (float)pGroup->iCount / (1024.0f*1024.0f);
						fMaxSize = (float)pGroup->iLimit / (1024.0f*1024.0f);
					}
					else
					{
						fSize = (float)pGroup->iCount;
						fMaxSize = (float)pGroup->iLimit;
					}

					if (pGroup->iCount > pGroup->iLimit && pGroup->iLimit >= 0)
					{
						GetRenderUtils()->Printf2D(fPosX, y, Graphics::O_RED, "Budget %s exceed %.2f / %.2f", pGroup->sName.c_str(), fSize, fMaxSize);
						y -= MENU_ENTRY_HIGH * 2;
					}
					else if (Core::Budget::GetCombo() == 2)
					{
						if (pGroup->iLimit > 0)
							GetRenderUtils()->Printf2D(fPosX, y, Graphics::O_WHITE, "Budget %s %.2f / %.2f", pGroup->sName.c_str(), fSize, fMaxSize);
						else
							GetRenderUtils()->Printf2D(fPosX, y, Graphics::O_WHITE, "Budget %s %.2f", pGroup->sName.c_str(), fSize);
						y -= MENU_ENTRY_HIGH * 2;
						y = RenderSubGroup(pGroup, fPosX, y);
					}
				}
			}
		}
	}*/

	void ImGuiRenderProfiler()
	{
		ImGui::SetNextWindowSize(ImVec2(480, 320), ImGuiSetCond_Appearing);
		if (ImGui::Begin("Profiler"))
		{
			//ImGuiRenderProfilerRoot();
		}
		ImGui::End();
	}

	/*
	void ImGuiRenderProfilerRoot()
	{
		Core::ProfilerBlock oBlkMain(Color32(0, 255, 0), "ImGui ProfilerBar");
		Core::ProfilerManager* pManager = Core::ProfilerManager::GetInstance();
		Core::Lock oLocker(pManager->GetCriticalSectionHandle());

		ImGuiIO& oIO = ImGui::GetIO();

		uint16 iCurrentThread = 0;
		Core::ProfilerHardwareThread* pThread = pManager->GetFirstHardwareThread();

		float fSplit = ImGui::GetCurrentWindow()->StateStorage.GetFloat(ImGui::GetID("Split"), 0.75f);
		static const float c_fBlockHeight = 20.f;
		const ImVec2 oContentSize = ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y * fSplit);
		const ImVec2 oContentMin = ImGui::GetWindowContentRegionMin();
		const ImVec2 oContentMax = ImGui::GetWindowContentRegionMax();
		if (ImGui::BeginChildFrame(ImGui::GetID("Timeline"), oContentSize, ImGuiWindowFlags_HorizontalScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
		{
			ImDrawList* pDrawList = ImGui::GetWindowDrawList();

			float fScale = ImGui::GetCurrentWindow()->StateStorage.GetFloat(ImGui::GetID("ProfilerScale"), 0.1f);

			// Draw 1ms lines
			static const float c_fDelta = 1000.f; // 1ms;
			static const float c_fSecondDelta = 5000.f; // 1ms;

			float fStart = ImGui::GetScrollX() / fScale;
			fStart = Math::Floor(fStart / c_fDelta) * c_fDelta;
			float fEnd = (ImGui::GetScrollX() + oContentSize.x) / fScale;
			ImU32 iColorGrey = ImGui::ColorConvertFloat4ToU32(ImVec4(0.3f, 0.3f, 0.3f, 1.f));
			ImU32 iColorWhite = ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 1.f, 1.f, 1.f));
			ImU32 oColorRed = ImGui::ColorConvertFloat4ToU32(ImVec4(1.f, 0.f, 0.f, 1.f));
			ImVec2 oCursorPos = ImGui::GetCurrentWindow()->DC.CursorPos;
			while (fStart <= fEnd)
			{
				float fX = fStart * fScale + oContentMin.x + oCursorPos.x;
				pDrawList->AddLine(ImVec2(fX, oContentMin.y + oCursorPos.y), ImVec2(fX, oContentMax.y + oCursorPos.y), (Math::Mod(fStart, c_fSecondDelta) == 0.f) ? iColorWhite : iColorGrey);
				fStart += c_fDelta;
			}

			// Draw 30/60 FPS line
			float fTime = 1000000.f / 60.f;
			float fX = fTime * fScale + oContentMin.x + oCursorPos.x;
			pDrawList->AddLine(ImVec2(fX, oContentMin.y), ImVec2(fX, oContentMax.y), oColorRed);
			fTime = 1000000.f / 30.f;
			fX = fTime * fScale + oContentMin.x + oCursorPos.x;
			pDrawList->AddLine(ImVec2(fX, oContentMin.y), ImVec2(fX, oContentMax.y), oColorRed);

			while (pThread)
			{
				if (pThread->GetMaxStackDepth() > 0)
				{
					ImGui::Dummy(ImVec2(ImGui::GetScrollX(), 1.f));
					ImGui::SameLine();
					ImGui::Text("Threads %d - %s", iCurrentThread, pThread->GetName());
					ImGui::Separator();
					oCursorPos = ImGui::GetCurrentWindow()->DC.CursorPos;
					float fMaxPosX = 0.f;
					const Core::ProfilerHardwareThread::BlockOutInfo* pBlock = pThread->GetBlockInfo();
					for (uint32 i = 0, iNbBlock = pThread->GetNbBlockInfo(); i < iNbBlock; ++i)
					{
						PA_ASSERT(pBlock->sLabel != NULL);

						ImRect oRect;
						oRect.Min = ImVec2(oContentMin.x + oCursorPos.x + pBlock->fStart * 1000000.f * fScale, oCursorPos.y + c_fBlockHeight * pBlock->iStackPos);
						oRect.Max = ImVec2(oContentMin.x + oCursorPos.x + (pBlock->fStart + pBlock->fLen) * 1000000.f * fScale, oCursorPos.y + c_fBlockHeight * (pBlock->iStackPos + 1));

						fMaxPosX = Math::Max(fMaxPosX, oRect.Max.x - oCursorPos.x);

						float fRed = PA_PR_COLOR_TO_RED(pBlock->oColor) / 255.f;
						float fGreen = PA_PR_COLOR_TO_GREEN(pBlock->oColor) / 255.f;
						float fBlue = PA_PR_COLOR_TO_BLUE(pBlock->oColor) / 255.f;

						float fHue, fSat, fVal;
						ImGui::ColorConvertRGBtoHSV(fRed, fGreen * 0.8f, fBlue * 0.2f, fHue, fSat, fVal);
						fHue += 0.5f;
						if (fHue > 1.f) fHue = fHue - 1.f;
						fVal = 1.f - fVal;
						float fCompRed, fCompGreen, fCompBlue;
						ImGui::ColorConvertHSVtoRGB(fHue, fSat, fVal, fCompRed, fCompGreen, fCompBlue);

						if (!ImGui::IsClippedEx(oRect, NULL, false))
						{
							pDrawList->AddRectFilled(oRect.Min, oRect.Max, ImGui::ColorConvertFloat4ToU32(ImVec4(fRed, fGreen, fBlue, 1.f)), 0.f);
							pDrawList->AddRect(oRect.Min, oRect.Max, ImGui::ColorConvertFloat4ToU32(ImVec4(fCompRed, fCompGreen, fCompBlue, 1.f)), 0.f);

							ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(fCompRed, fCompGreen, fCompBlue, 1.f));
							ImGui::RenderTextClipped(oRect.Min, oRect.Max, pBlock->sLabel, NULL, NULL, ImGuiAlign_Center);
							ImGui::PopStyleColor();

							if (ImGui::IsMouseHoveringRect(oRect.Min, oRect.Max, false))
							{
								ImGui::BeginTooltip();
								float fMs = pBlock->fLen * 1000.f;
								ImGui::Text(
									"%s\n"
									"Time: %.2g ms\n",
									pBlock->sLabel,
									fMs);
								ImGui::EndTooltip();
							}
						}

						++pBlock;
					}

					ImGui::Dummy(ImVec2(fMaxPosX, c_fBlockHeight * (pThread->GetMaxStackDepth() + 0.5f)));
				}

				pThread = pManager->GetNextHardwareThread();
				++iCurrentThread;
			}

			if (ImGui::IsWindowHovered() && oIO.MouseWheel != 0)
			{
				float fScroll = ImGui::GetScrollX();

				float fMult = 1.f;
				if (oIO.MouseWheel < 0)
				{
					fMult = 0.9f;
				}
				else
				{
					fMult = 1.1f;
				}

				float fOldScale = fScale;
				fScale *= fMult;

				//float fWidth = ImGui::GetContentRegionAvail().x;
				float fWidth = oContentSize.x;

				float fMid = (oIO.MousePos.x - ImGui::GetWindowPos().x) / fWidth;
				float fTime = (fScroll + fWidth * fMid) / fOldScale;

				ImGui::SetScrollX((fTime * fScale) - fWidth * fMid);

				ImGui::GetCurrentWindow()->StateStorage.SetFloat(ImGui::GetID("ProfilerScale"), fScale);
			}

			if (ImGui::IsWindowHovered() && !ImGui::IsAnyItemActive() && ImGui::IsMouseDragging(0, 0.0f))
			{
				ImGui::SetScrollX(ImGui::GetScrollX() - oIO.MouseDelta.x);
				ImGui::SetScrollY(ImGui::GetScrollY() - oIO.MouseDelta.y);
			}
		}
		ImGui::EndChildFrame();

		ImGui::Button("##SplitDrag", ImVec2(ImGui::GetWindowSize().x, 3.f));
		if (ImGui::IsItemActive())
		{
			fSplit += oIO.MouseDelta.y / ImGui::GetWindowSize().y;
			fSplit = ImClamp(fSplit, 0.05f, 0.95f);
			ImGui::GetCurrentWindow()->StateStorage.SetFloat(ImGui::GetID("Split"), fSplit);
		}

		if (ImGui::BeginChildFrame(ImGui::GetID("Tree"), ImVec2(ImGui::GetContentRegionAvail().x, ImGui::GetContentRegionAvail().y)))
		{
			pThread = pManager->GetFirstHardwareThread();
			while (pThread)
			{
				if (pThread->GetNbBlockInfo() > 0)
				{
					if (ImGui::CollapsingHeader(pThread->GetName()))
					{
						uint iCurrentStack = 0;
						Array<float> vParentTime;
						ImGui::Columns(3);
						ImGui::TextDisabled("Block");
						ImGui::NextColumn();
						ImGui::TextDisabled("Time");
						ImGui::NextColumn();
						ImGui::TextDisabled("Percent");
						ImGui::NextColumn();
						const Core::ProfilerHardwareThread::BlockOutInfo* pBlock = pThread->GetBlockInfo();
						for (uint32 i = 0, iNbBlock = pThread->GetNbBlockInfo(); i < iNbBlock; ++i)
						{
							uint iCurrentStackPos = pBlock->iStackPos;
							int iPopCount = iCurrentStack - iCurrentStackPos;
							while (iPopCount >= 0 && !vParentTime.empty())
							{
								vParentTime.pop_back();
								--iPopCount;
							}
							iCurrentStack = iCurrentStackPos;
							ImGui::Dummy(ImVec2(5.f * iCurrentStackPos, 1.f));
							ImGui::SameLine();
							bool bOpen = ImGui::TreeNode(pBlock->sLabel);
							if (bOpen)
							{
								ImGui::TreePop();
							}
							ImGui::NextColumn();
							ImGui::Text("%.2g ms", pBlock->fLen * 1000.f);
							ImGui::NextColumn();
							float fParentTime = vParentTime.empty() ? pBlock->fLen : vParentTime.back();
							ImGui::Text("%.2f%%", 100.f * (pBlock->fLen / fParentTime));
							ImGui::NextColumn();
							vParentTime.push_back(pBlock->fLen);
							if (!bOpen)
							{
								++i;
								++pBlock;
								while (i < iNbBlock && pBlock->iStackPos > iCurrentStackPos)
								{
									++i;
									++pBlock;
								}
								--i;
								--pBlock;
							}
							++pBlock;
						}
						ImGui::Columns(1);
					}
					ImGui::Separator();
				}
				pThread = pManager->GetNextHardwareThread();
			}
		}
		ImGui::EndChildFrame();
	}*/
}