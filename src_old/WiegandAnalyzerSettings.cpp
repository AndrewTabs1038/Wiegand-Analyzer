#include "WiegandAnalyzerSettings.h"
#include <AnalyzerHelpers.h>


WiegandAnalyzerSettings::WiegandAnalyzerSettings()
:	D0Channel( UNDEFINED_CHANNEL ),
	D1Channel( UNDEFINED_CHANNEL ), 
	site_length(0),
	facility_length(8), 
	cardid_length( 16 )
{
    D0ChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
    D0ChannelInterface->SetTitleAndTooltip( "D0", "Standard WiegandD0" );
    D0ChannelInterface->SetChannel( D0Channel );

	D1ChannelInterface.reset( new AnalyzerSettingInterfaceChannel() );
    D1ChannelInterface->SetTitleAndTooltip( "D1", "Standard WiegandD1" );
    D1ChannelInterface->SetChannel( D1Channel );

	site_lengthInterface.reset( new AnalyzerSettingInterfaceInteger() );
    site_lengthInterface->SetTitleAndTooltip( "# of Bits in Site", "Specify length of Site" );
    site_lengthInterface->SetMax( 100 );
    site_lengthInterface->SetMin( 0 );
    site_lengthInterface->SetInteger( site_length );

	facility_lengthInterface.reset( new AnalyzerSettingInterfaceInteger() );
    facility_lengthInterface->SetTitleAndTooltip( "# of facility Bits", "Specify length of Site" );
    facility_lengthInterface->SetMax( 100 );
    facility_lengthInterface->SetMin( 0 );
    facility_lengthInterface->SetInteger( facility_length );

	cardid_lengthInterface.reset( new AnalyzerSettingInterfaceInteger() );
    cardid_lengthInterface->SetTitleAndTooltip( "# of Card Number Bits", "Specify length of Site" );
    cardid_lengthInterface->SetMax( 100 );
    cardid_lengthInterface->SetMin( 0 );
    cardid_lengthInterface->SetInteger( cardid_length );

	AddInterface( D0ChannelInterface.get() );
    AddInterface( D1ChannelInterface.get() );
    AddInterface( site_lengthInterface.get() );
    AddInterface( facility_lengthInterface.get() );
    AddInterface( cardid_lengthInterface.get() );

	AddExportOption( 0, "Export as text/csv file" );
	AddExportExtension( 0, "text", "txt" );
	AddExportExtension( 0, "csv", "csv" );

	ClearChannels();
    AddChannel( D0Channel, "D0", true );
    AddChannel( D1Channel, "D1", true );
}

WiegandAnalyzerSettings::~WiegandAnalyzerSettings()
{
}

bool WiegandAnalyzerSettings::SetSettingsFromInterfaces()
{
	D0Channel = D0ChannelInterface->GetChannel();
	D1Channel = D1ChannelInterface->GetChannel();
    site_length = site_lengthInterface->GetInteger();
    facility_length = facility_lengthInterface->GetInteger();
    cardid_length = cardid_lengthInterface->GetInteger();

	ClearChannels();
	AddChannel( D0Channel, "D0", true );
    AddChannel( D0Channel, "D1", true );

	return true;
}

void WiegandAnalyzerSettings::UpdateInterfacesFromSettings()
{
	D0ChannelInterface->SetChannel( D0Channel );
	D1ChannelInterface->SetChannel( D1Channel );
    site_length = site_lengthInterface->GetInteger();
    facility_length = facility_lengthInterface->GetInteger();
    cardid_length = cardid_lengthInterface->GetInteger();
}

void WiegandAnalyzerSettings::LoadSettings( const char* settings )
{
	SimpleArchive text_archive;
	text_archive.SetString( settings );

	 const char* name_string; 
	 text_archive >> &name_string; //checks if the first thing in the archive is the name of the protocl analyzer the data belongs to
	 if( strcmp( name_string, "SaleaeWiegandAnalyzer" ) != 0 )
        AnalyzerHelpers::Assert( "SaleaeWiegandAnalyzer: Provided with a settings string that doesn't belong to us;" );

	text_archive >> D0Channel;
    text_archive >> D1Channel;
    text_archive >> site_length;
    text_archive >> facility_length;
    text_archive >> cardid_length;
    
	
	ClearChannels();
    AddChannel( D0Channel, "D0", true );
    AddChannel( D1Channel, "D1", true );

	UpdateInterfacesFromSettings();
}

const char* WiegandAnalyzerSettings::SaveSettings()
{
	SimpleArchive text_archive;


	text_archive << "SaleaeWiegandAnalyzer";
    text_archive << D0Channel;
    text_archive << D1Channel;
    text_archive << site_length;
    text_archive << facility_length;
    text_archive << cardid_length;


	return SetReturnString( text_archive.GetString() );
}
