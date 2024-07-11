#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
#include "WiegandAnalyzer.h"
#include "WiegandAnalyzerSettings.h"
#include <AnalyzerChannelData.h>

WiegandAnalyzer::WiegandAnalyzer()
:	Analyzer2(),  
	mSettings( new WiegandAnalyzerSettings() ),
	mSimulationInitilized( false )
{
	SetAnalyzerSettings( mSettings.get() );
}

WiegandAnalyzer::~WiegandAnalyzer()
{
	KillThread();
}

void WiegandAnalyzer::SetupResults()
{
	mResults.reset( new WiegandAnalyzerResults( this, mSettings.get() ) );
	SetAnalyzerResults( mResults.get() );
	mResults->AddChannelBubblesWillAppearOn( mSettings->D0Channel );
}

U64 binaryTotal( vector<U64> temp )
{
    U64 total = 0;
    vector<U64> binary;

    for( int i = temp.size() - 1; i >= 0; i-- )
    {
        binary.push_back( temp[ i ] );
    }
    for( int i = 0; i < binary.size(); i++ )
    {
        if( binary[ i ] == 1 )
        {
            total += pow( 2, i );
        }
    }

    return total;
}

void WiegandAnalyzer::WorkerThread()
{	

    U32 length = 0;

    U32 site_length = mSettings->site_length;

    U32 facility_length = mSettings->facility_length;

    U32 cardid_length = mSettings->cardid_length;
   
    U32 counter = 0;

    U8 type = 1;
    

   length = site_length + facility_length + cardid_length + 2;

    vector<U64> temp; 
    vector<U64> site_bits;
    vector<U64> facility_bits;
    vector<U64> card_bits;

	D0Serial = GetAnalyzerChannelData( mSettings->D0Channel );
	D1Serial = GetAnalyzerChannelData( mSettings->D1Channel );


	U64 positionD0 = 0;
	U64 positionD1 = 0;

    U64 firstBit_parity = 0;
    U64 lastBit_parity = 0; 
    U64 firstBit_site = 0;
    U64 lastBit_site = 0;
    U64 firstBit_facility = 0;
    U64 lastBit_facility = 0;
    U64 firstBit_card = 0;
    U64 lastBit_card = 0;
    

    U64 firstBit = 0;
    U64 lastBit= 0; 
    
    
    U64 firstBit_in = 0;
    U64 lastBit_in= 0;   

	bool detect_first = true;
    bool detect_last = true;

    bool first = true;
    bool last = true;
    bool site_processed = false;
    bool facility_processed = false;
    bool card_processed = false;

	U64 data = 0;

	for( ; ; )
	{
     

		if (detect_first) {

            
           positionD0 = D0Serial->GetSampleOfNextEdge();
            positionD1 = D1Serial->GetSampleOfNextEdge();
            

			if( positionD0 < positionD1 )
            {
                firstBit_parity = positionD0;
                D0Serial->AdvanceToNextEdge();
                D0Serial->AdvanceToNextEdge();
                lastBit_parity = D0Serial->GetSampleNumber();
                mResults->AddMarker( D0Serial->GetSampleNumber(), AnalyzerResults::Zero, mSettings->D0Channel );
                temp.push_back( 0 );
            }
            else 
            {
                firstBit_parity = positionD1;
                D1Serial->AdvanceToNextEdge();
                D1Serial->AdvanceToNextEdge();
                lastBit_parity = D1Serial->GetSampleNumber();
                mResults->AddMarker( D1Serial->GetSampleNumber(), AnalyzerResults::One, mSettings->D1Channel );
                temp.push_back( 1 );
                
            }

           
            counter++;


            detect_first = false;
		}

		 if( detect_last )
        {
            while( counter < length  && (D0Serial->DoMoreTransitionsExistInCurrentData() || D1Serial->DoMoreTransitionsExistInCurrentData()))
            {
             

                if( D0Serial->DoMoreTransitionsExistInCurrentData() && D1Serial->DoMoreTransitionsExistInCurrentData() )
               {
                   if( D0Serial->GetSampleOfNextEdge() < D1Serial->GetSampleOfNextEdge() )
                   {
                       D0Serial->AdvanceToNextEdge();
                       temp.push_back( 0 );
                       D0Serial->AdvanceToNextEdge();
                       counter++;
                       mResults->AddMarker( D0Serial->GetSampleNumber(), AnalyzerResults::Zero, mSettings->D0Channel );

                   }
                   else if( D0Serial->GetSampleOfNextEdge() > D1Serial->GetSampleOfNextEdge() )
                   {
                       D1Serial->AdvanceToNextEdge();
                       temp.push_back( 1 );
                       D1Serial->AdvanceToNextEdge();
                       counter++;
                       mResults->AddMarker( D1Serial->GetSampleNumber(), AnalyzerResults::One, mSettings->D1Channel );
                   }
               }
               else if( D0Serial->DoMoreTransitionsExistInCurrentData() )
               {
                   D0Serial->AdvanceToNextEdge();
                   temp.push_back( 0 );
                   D0Serial->AdvanceToNextEdge();
                   counter++;
                   mResults->AddMarker( D0Serial->GetSampleNumber(), AnalyzerResults::Zero, mSettings->D0Channel );

                }
               else if (D1Serial->DoMoreTransitionsExistInCurrentData()) {
                   D1Serial->AdvanceToNextEdge();
                   temp.push_back( 1 );
                   D1Serial->AdvanceToNextEdge();
                   counter++;
                   mResults->AddMarker( D1Serial->GetSampleNumber(), AnalyzerResults::One, mSettings->D1Channel );

                }

                if( counter == 2 && site_length != 0 )
                {
                    positionD0 = D0Serial->GetSampleNumber();
                    positionD1 = D1Serial->GetSampleNumber();

                    if( positionD0 > positionD1 )
                    {
                        firstBit_site = positionD0;
                    }
                    else
                    {
                        firstBit_site = positionD1;
                    }
                }
               
                if( counter == site_length + 1 && site_length != 0)
                {
                    positionD0 = D0Serial->GetSampleNumber();            
                    positionD1 = D1Serial->GetSampleNumber();  
                    
                    if (positionD0 > positionD1) {
                        lastBit_site = positionD0;
                    }
                    else
                    {
                        lastBit_site = positionD1;
                    }

                }
                else if( counter == site_length + 2 && facility_length != 0)
                {
                    positionD0 = D0Serial->GetSampleNumber();
                    positionD1 = D1Serial->GetSampleNumber();

                    if( positionD0 > positionD1 )
                    {
                        firstBit_facility = positionD0;
                    }
                    else
                    {
                        firstBit_facility = positionD1;
                    }
                }

                else if( counter == facility_length + site_length + 1 && facility_length != 0 )
                {
                    positionD0 = D0Serial->GetSampleNumber();
                    positionD1 = D1Serial->GetSampleNumber();

                    if( positionD0 > positionD1 )
                    {
                        lastBit_facility = positionD0;
                    }
                    else
                    {
                        lastBit_facility = positionD1;
                    }


                }
                else if (counter == facility_length + site_length + 2)
                {
                    positionD0 = D0Serial->GetSampleNumber();
                    positionD1 = D1Serial->GetSampleNumber();

                    if( positionD0 > positionD1 )
                    {
                        firstBit_card = positionD0;
                    }
                    else
                    {
                        firstBit_card = positionD1;
                    }


                } 
                else if( counter == cardid_length + facility_length + site_length + 1)
                {
                    positionD0 = D0Serial->GetSampleNumber();
                    positionD1 = D1Serial->GetSampleNumber();

                    if( positionD0 > positionD1 )
                    {
                        lastBit_card = positionD0;
                    }
                    else
                    {
                        lastBit_card = positionD1;
                    }
                   

                }
               else if( counter == length)
                {
                    positionD0 = D0Serial->GetSampleNumber();
                    positionD1 = D1Serial->GetSampleNumber();

                    if( positionD0 > positionD1 )
                    {
                        firstBit = positionD0;
                        lastBit = positionD0 + lastBit_parity - firstBit_parity;
                    }
                    else
                    {
                        firstBit = positionD1;
                        lastBit = positionD1 + lastBit_parity - firstBit_parity;

                    }
                }
                


            }


            detect_last = false;
        }
        else 
        {

            if (first) {
                data = temp[ 0 ];
                temp[ 0 ] = 0;
                firstBit_in = firstBit_parity;
                lastBit_in = lastBit_parity;
                first = false;
                type = 1;

            }
            else if( !site_processed && site_length != 0)
            {
                for( int i = 1; i < site_length + 1; i++ )
                {
                    site_bits.push_back( temp[i] );
                    temp[ i ] = 0;
                }
                firstBit_in = firstBit_site;
                lastBit_in = lastBit_site;
                data = binaryTotal( site_bits );
                site_processed = true;
                type = 2;
            }
            else if( !facility_processed && facility_length != 0)
            {
                for( int i = 1 + site_length; i < facility_length + 1 + site_length; i++ )
                {
                    facility_bits.push_back( temp[i] );
                    temp[ i ] = 0;
                }
                firstBit_in = firstBit_facility;
                lastBit_in = lastBit_facility;
                data = binaryTotal( facility_bits );
                facility_processed = true;  
                type = 3;
            }
            else if( !card_processed  &&  cardid_length != 0)
            {
                for( int i = 1 + site_length + facility_length; i < cardid_length + facility_length + 1 + site_length; i++ )
                {
                    card_bits.push_back( temp[ i ] );
                    temp[ i ] = 0;
                }
                firstBit_in = firstBit_card;
                lastBit_in = lastBit_card;
                data = binaryTotal( card_bits );
                card_processed = true;
                type = 4;
            }

            else if(last)
            {
                data = binaryTotal( temp );
                firstBit_in = firstBit;
                lastBit_in = lastBit;
                last = false;
                type = 5;


                detect_first = true;
                detect_last = true;

                first = true;
                last = true;
                site_processed = false;
                facility_processed = false;
                card_processed = false;

                counter = 0;
                temp.clear();
                site_bits.clear();
                facility_bits.clear();
                card_bits.clear();
            }

            Frame frame;
            frame.mData1 = data;
            frame.mFlags = 0;
            frame.mType = type;
            frame.mStartingSampleInclusive = firstBit_in;
            frame.mEndingSampleInclusive = lastBit_in;

            mResults->AddFrame( frame );
            mResults->CommitResults();
            ReportProgress( frame.mEndingSampleInclusive );
            CheckIfThreadShouldExit();
        }
	}
}



bool WiegandAnalyzer::NeedsRerun()
{
	return false;
}

U32 WiegandAnalyzer::GenerateSimulationData( U64 minimum_sample_index, U32 device_sample_rate, SimulationChannelDescriptor** simulation_channels )
{
	if( mSimulationInitilized == false )
	{
		mSimulationDataGenerator.Initialize( GetSimulationSampleRate(), mSettings.get() );
		mSimulationInitilized = true;
	}

	return mSimulationDataGenerator.GenerateSimulationData( minimum_sample_index, device_sample_rate, simulation_channels );
}

U32 WiegandAnalyzer::GetMinimumSampleRateHz()
{
    return 25000;
}

const char* WiegandAnalyzer::GetAnalyzerName() const
{
	return "Wiegand";
}

const char* GetAnalyzerName()
{
	return "Wiegand";
}

Analyzer* CreateAnalyzer()
{
	return new WiegandAnalyzer();
}

void DestroyAnalyzer( Analyzer* analyzer )
{
	delete analyzer;
}
