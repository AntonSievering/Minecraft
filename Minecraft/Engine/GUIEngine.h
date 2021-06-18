#include "Engine.h"
#include "GUI/UserInterface.h"

namespace Engine
{
	class GUIEngine : public PixelEngine
	{
	private:
		class MainUserInterface : public GUI::UserInterface
		{
		private:
			GUIEngine *m_pEngine = nullptr;

		public:
			MainUserInterface() noexcept : UserInterface() {};

			MainUserInterface(const vf2d &pos, const vf2d &size, GUIEngine *e) noexcept
			{
				this->pos = pos;
				this->size = size;
				m_pEngine = e;
			}

			~MainUserInterface() noexcept override {}

		public:
			void render() noexcept
			{
				if (getDerived() != nullptr)
					getDerived()->render(m_pEngine);
			}
		};

	private:
		MainUserInterface m_uiMainUserInterface = MainUserInterface({ -1.0f, -1.0f }, { 2.0f, 2.0f }, this);

	public:
		GUIEngine() noexcept : PixelEngine() {};

		// GUI Integration
	public:
		void attachUserInterface(GUI::UserInterface *pUi) noexcept
		{
			m_uiMainUserInterface.getEndpoint()->derive(pUi);
		}

		HWKey GetKey(const Key &key) const noexcept override
		{
			return m_uiMainUserInterface.getKey(key);
		}

	private:
		virtual void UpdateInternals() noexcept override
		{
			m_uiMainUserInterface.getEndpoint()->setKeys(m_vKeyManager);
			m_uiMainUserInterface.update(this);
		}

		virtual void RenderInternals() noexcept override
		{
			Start2D();
			m_uiMainUserInterface.render();
			End2D();
		}
	};
}
