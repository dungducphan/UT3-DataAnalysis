#include "ScopeManager.h"

void StartUp (void) {
	int8_t	ch;

	printf ( "PS3000 driver example program\n" );
	printf ( "Version 1.2\n\n" );

	printf ( "\n\nOpening the device...\n");

	//open unit and show splash screen
	unitOpened.handle = ps3000_open_unit ();
	printf ( "Handle: %d\n", unitOpened.handle );

	if ( unitOpened.handle == 0 )
	{
		printf ( "Unable to open device\n" );
		printf ( "Device not found\n" );

		while( !_kbhit() );
		exit ( 99 );
	}
	else if ( unitOpened.handle < 0 )
	{
		printf ( "Unable to open device\n" );
		printf ( "Error occurred\n" );
		get_info ();
		while( !_kbhit() );
		exit ( 99 );
	}
	else
	{
		printf ( "Device opened successfully\n\n" );
		get_info ();

		timebase = 0;
		ch = ' ';

		while ( ch != 'X' )
		{
			printf ( "\n" );
			printf ( "B - immediate block				V - Set voltages\n" );
			printf ( "T - triggered block				I - Set timebase\n" );
			printf ( "Y - advanced triggered block		A - ADC counts/mV\n" );
			printf ( "E - ETS block\n" );
			printf ( "S - Streaming\n" );
			printf ( "F - Fast streaming\n");
			printf ( "D - Fast streaming triggered\n" );
			printf ( "G - Toggle signal generator on/off\n" );
			printf ( "X - exit\n" );
			printf ( "Operation:" );

			ch = toupper ( _getch () );

			printf ( "\n\n" );
			switch ( ch )
			{
			case 'B':
				collect_block_immediate ();
				break;

			case 'T':
				collect_block_triggered ();
				break;

			case 'Y':
				if (unitOpened.hasAdvancedTriggering)
				{
					collect_block_advanced_trigger ();
				}
				else
				{
					printf ("Not supported by this model\n\n");
				}
				break;


			case 'S':
				collect_streaming ();
				break;

			case 'F':
				if (unitOpened.hasFastStreaming)
				{
					collect_fast_streaming ();
				}
				else
				{
					printf ("Not supported by this model\n\n");
				}
				break;

			case 'D':
				if (unitOpened.hasFastStreaming && unitOpened.hasAdvancedTriggering)
				{
					collect_fast_streaming_triggered ();
				}
				else
				{
					printf ("Not supported by this model\n\n");
				}
				break;

			case 'G':
				set_signal_generator ();
				break;

			case 'E':
				collect_block_ets ();
				break;

			case 'V':
				set_voltages ();
				break;

			case 'I':
				set_timebase ();
				break;

			case 'A':
				scale_to_mv = !scale_to_mv;
				if ( scale_to_mv )
				{
					printf ( "Readings will be scaled in mV\n" );
				}
				else
				{
					printf ( "Readings will be scaled in ADC counts\n" );
				}
				break;

			case 'X':
				/* Handled by outer loop */
				break;

			default:
				printf ( "Invalid operation\n" );
				break;
			}
		}

		ps3000_close_unit ( unitOpened.handle );
	}
}