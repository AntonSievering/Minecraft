#pragma once

#include "Alignment.h"
#include "Widget.h"

namespace Engine
{
	namespace GUI
	{
		class Label : public Widget
		{
		private:
			StringDecal m_stringDecal{};
			Pixel       m_pxTextColour{};
			float       m_fOffsetX{};
			float       m_fTextSize{};

		public:
			Label() noexcept : Widget() {}

			Label(const vf2d &vPos, const vf2d &vSize, const std::string &sText, const Pixel &pxCol, const Alignment &alignment) noexcept
				: Widget(vPos, vSize)
			{
				m_pxTextColour = pxCol;
				setText(sText, alignment);
			}

		public:
			void setText(const std::string &sText, const Alignment &alignment) noexcept
			{
				if (sText.size() > 0)
				{
					vf2d vTextSize = PixelEngine::GetStringSize(sText);
					m_fTextSize = size.y / vTextSize.y;

					switch (alignment)
					{
					case Alignment::LEFT:
						m_fOffsetX = 0.0f;
						break;
					case Alignment::RIGHT:
						m_fOffsetX = size.x - m_fTextSize * vTextSize.x;
						break;
					case Alignment::CENTER:
						m_fOffsetX = size.x - 0.5f * m_fTextSize * vTextSize.x;
						break;
					}

					std::string sFinalText{};

					if (m_fTextSize * vTextSize.x > size.x)
					{
						float fFittingFraction = size.x / (m_fTextSize * vTextSize.x);
						int32_t nFittingChars = (int32_t)std::floor(fFittingFraction * sText.size());
						sFinalText = sText.substr(0, std::max(nFittingChars - 3, 0));
						sFinalText.append("...");
					}
					else
					{
						sFinalText = sText;
					}

					m_stringDecal = StringDecal(sFinalText, vf2d(pos.x + m_fOffsetX, pos.y), m_fTextSize);
				}
			}

			virtual void render(PixelEngine *engine, bool bActiveGUI, const KeyManager &keyManager) noexcept override
			{
				engine->DrawStringDecal(m_stringDecal, m_pxTextColour, Resolution::HIGH);
			}
		};
	}
}
