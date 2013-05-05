#include <znc/main.h>
#include <znc/Modules.h>
#include <znc/User.h>
#include <znc/Chan.h>
#include <znc/IRCNetwork.h>


class CWelcomeTimer: public CTimer {
private:
	CString mNick;

public:
	CWelcomeTimer(CModule* pModule, unsigned int uInterval, unsigned int uCycles, const CString& sLabel, const CString& sDescription,
		const CString &sNick)
		: CTimer(pModule, uInterval, uCycles, sLabel, sDescription),
		  mNick(sNick)
		  {}

	virtual ~CWelcomeTimer() {}

protected:
	virtual void RunJob();
};


class CWelcomeToPassenger: public CModule {
public:
	MODCONSTRUCTOR(CWelcomeToPassenger) {}

	virtual ~CWelcomeToPassenger() {}

	virtual void OnJoin(const CNick& Nick, CChan& Channel) {
		if (Channel.GetName() == "#passenger" && Nick.GetNick() != m_pNetwork->GetIRCNick().GetNick()) {
			AddTimer(new CWelcomeTimer(this, 5, 1,
				"welcome_to_passenger", "Welcomes " + Nick.GetNick(),
				Nick.GetNick()));
		}
	}

	void Welcome(const CString &sNick) {
		PutIRC("NOTICE " + sNick + " :Hi " + sNick +
			", welcome to #passenger. Please feel free to hang around and chat with people. "
			"The authors (e.g. FooBarWidget) are not always watching the channel and are in "
			"timezone Europe/Amsterdam. If you want to nudge him, begin your message with "
			"'FooBarWidget'. If nobody responds, please be patient, or see "
			"www.phusionpassenger.com/support. Enjoy :)");
	}
};


void CWelcomeTimer::RunJob() {
	((CWelcomeToPassenger *) m_pModule)->Welcome(mNick);
}


template<> void TModInfo<CWelcomeToPassenger>(CModInfo& Info) {
	Info.SetWikiPage("welcome_to_passenger");
	Info.SetHasArgs(false);
}

NETWORKMODULEDEFS(CWelcomeToPassenger, "Welcome people to #passenger.")
