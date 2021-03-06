#ifndef _PH_DI_INPUTMANAGER_H_
#define _PH_DI_INPUTMANAGER_H_

#include "directinput/Manager.h"
#include "tcharx.h"
#include "Types.h"
#include "Config.h"
#include "../../ControllerInfo.h"
#include <memory>
#include <functional>
#include "boost/signals2.hpp"

namespace PH_DirectInput
{
	class CInputManager
	{
	public:
		enum BINDINGTYPE
		{
			BINDING_UNBOUND = 0,
			BINDING_SIMPLE = 1,
			BINDING_SIMULATEDAXIS = 2,
			BINDING_POVHAT = 3,
		};

		struct BINDINGINFO
		{
			BINDINGINFO(const GUID& device, uint32 id) : device(device), id(id) { }
			GUID		device;
			uint32		id;
		};

		class CBinding
		{
		public:
			virtual							~CBinding() {};

			virtual BINDINGTYPE				GetBindingType() const = 0;

			virtual std::tstring			GetDescription(Framework::DirectInput::CManager*) const = 0;
			virtual void					ProcessEvent(const GUID&, uint32, uint32) = 0;

			virtual uint32					GetValue() const = 0;
			virtual void					SetValue(uint32) = 0;

			virtual void					Save(Framework::CConfig&, const char*) const = 0;
			virtual void					Load(Framework::CConfig&, const char*) = 0;
		};

		class CSimpleBinding : public CBinding
		{
		public:
											CSimpleBinding(const GUID& = GUID(), uint32 = 0);
			virtual							~CSimpleBinding();

			static void						RegisterPreferences(Framework::CConfig&, const char*);

			virtual BINDINGTYPE				GetBindingType() const;

			virtual std::tstring			GetDescription(Framework::DirectInput::CManager*) const;
			virtual void					ProcessEvent(const GUID&, uint32, uint32);

			virtual uint32					GetValue() const;
			virtual void					SetValue(uint32);

			virtual void					Save(Framework::CConfig&, const char*) const;
			virtual void					Load(Framework::CConfig&, const char*);

		private:
			BINDINGINFO						m_binding;
			uint32							m_value;
		};

		class CPovHatBinding : public CBinding
		{
		public:
											CPovHatBinding(const GUID& = GUID(), uint32 = 0, uint32 = -1);
			virtual							~CPovHatBinding();

			static void						RegisterPreferences(Framework::CConfig&, const char*);

			virtual BINDINGTYPE				GetBindingType() const;

			virtual std::tstring			GetDescription(Framework::DirectInput::CManager*) const;
			virtual void					ProcessEvent(const GUID&, uint32, uint32);

			virtual uint32					GetValue() const;
			virtual void					SetValue(uint32);

			virtual void					Save(Framework::CConfig&, const char*) const;
			virtual void					Load(Framework::CConfig&, const char*);

		private:
			static int32					GetShortestDistanceBetweenAngles(int32, int32);

			BINDINGINFO						m_binding;
			uint32							m_refValue;
			uint32							m_value;
		};

		class CSimulatedAxisBinding : public CBinding
		{
		public:
											CSimulatedAxisBinding(const BINDINGINFO&, const BINDINGINFO&);
			virtual							~CSimulatedAxisBinding();

			static void						RegisterPreferences(Framework::CConfig&, const char*);

			virtual BINDINGTYPE				GetBindingType() const;

			virtual std::tstring			GetDescription(Framework::DirectInput::CManager*) const;
			virtual void					ProcessEvent(const GUID&, uint32, uint32);

			virtual uint32					GetValue() const;
			virtual void					SetValue(uint32);

			virtual void					Save(Framework::CConfig&, const char*) const;
			virtual void					Load(Framework::CConfig&, const char*);

		private:
			BINDINGINFO						m_key1Binding;
			BINDINGINFO						m_key2Binding;

			uint32							m_key1State;
			uint32							m_key2State;
		};

											CInputManager(HWND, Framework::CConfig&);
		virtual								~CInputManager();

		uint32								GetBindingValue(PS2::CControllerInfo::BUTTON) const;
		void								ResetBindingValues();

		const CBinding*						GetBinding(PS2::CControllerInfo::BUTTON) const;
		void								SetSimpleBinding(PS2::CControllerInfo::BUTTON, const BINDINGINFO&);
		void								SetPovHatBinding(PS2::CControllerInfo::BUTTON, const BINDINGINFO&, uint32);
		void								SetSimulatedAxisBinding(PS2::CControllerInfo::BUTTON, const BINDINGINFO&, const BINDINGINFO&);

		void								Load();
		void								Save();
		void								AutoConfigureKeyboard();

		Framework::DirectInput::CManager*	GetDirectInputManager() const;
		std::tstring						GetBindingDescription(PS2::CControllerInfo::BUTTON) const;

	private:
		typedef std::shared_ptr<CBinding> BindingPtr;

		void								OnInputEventReceived(const GUID&, uint32, uint32);

		static void							RegisterBindingPreference(Framework::CConfig&, const char*);

		BindingPtr							m_bindings[PS2::CControllerInfo::MAX_BUTTONS];
		static uint32						m_buttonDefaultValue[PS2::CControllerInfo::MAX_BUTTONS];

		Framework::DirectInput::CManager*	m_directInputManager;
		Framework::CConfig&					m_config;
	};
}

#endif
