#ifndef WIEGAND_ANALYZER_SETTINGS
#define WIEGAND_ANALYZER_SETTINGS

#include <AnalyzerSettings.h>
#include <AnalyzerTypes.h>

class WiegandAnalyzerSettings : public AnalyzerSettings
{
public:
	WiegandAnalyzerSettings();
	virtual ~WiegandAnalyzerSettings();

	virtual bool SetSettingsFromInterfaces();
	void UpdateInterfacesFromSettings();
	virtual void LoadSettings( const char* settings );
	virtual const char* SaveSettings();

	Channel D0Channel;
    Channel D1Channel;
    U32 site_length;
    U32 facility_length;
    U32 cardid_length;

protected:
	std::unique_ptr<AnalyzerSettingInterfaceChannel> D0ChannelInterface;
	std::unique_ptr<AnalyzerSettingInterfaceChannel> D1ChannelInterface;
	std::unique_ptr<AnalyzerSettingInterfaceInteger> site_lengthInterface;
	std::unique_ptr<AnalyzerSettingInterfaceInteger> facility_lengthInterface;
	std::unique_ptr<AnalyzerSettingInterfaceInteger> cardid_lengthInterface;

};

#endif //WIEGAND_ANALYZER_SETTINGS
