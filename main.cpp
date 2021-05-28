#include "main.h"

auto console::text::color( int color ) -> void {
	SetConsoleTextAttribute( GetStdHandle( STD_OUTPUT_HANDLE ), color );
}

auto randomization( ) -> int {
	if ( random::clicks >= random::reset_value || random::clicks == 0 ) {
		random::reset_value = random_int( 1, 5 );
		random::edited_cps = config::autoclicker::left_cps + random_int( -3, 3 );

		random::clicks = 0;
	}
	random::clicks++;

	random::return_ms = random_int( 225, 275 );

	return random::return_ms / random::edited_cps * 2;
}

auto send_click( ) -> void {
	std::this_thread::sleep_for( std::chrono::milliseconds( randomization( ) ) );

	PostMessage( GetForegroundWindow( ), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM( config::autoclicker::pos.x, config::autoclicker::pos.y ) );

	std::this_thread::sleep_for( std::chrono::milliseconds( randomization( ) ) );

	PostMessage( GetForegroundWindow( ), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM( config::autoclicker::pos.x, config::autoclicker::pos.y ) );
}

auto launch::thread::autoclicker( ) {
	while ( true ) {
		if ( config::autoclicker::enabled && GetAsyncKeyState( VK_LBUTTON ) & 0x8000 ) {
			if ( random_int( 0, 100 ) <= 2 )
				std::this_thread::sleep_for( std::chrono::milliseconds( randomization( ) * 3 ) );

			if ( GetForegroundWindow( ) == FindWindowA( xorstr( "LWJGL" ), NULL ) && ScreenToClient( GetForegroundWindow( ), &config::autoclicker::pos ) ) {
				send_click( );
			}
		}
		else {
			std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
		}
	}
}

auto launch::thread::jitter( ) {
	while ( true ) {
		if (  GetForegroundWindow( ) == FindWindowA( xorstr( "LWJGL" ), NULL ) && ScreenToClient( GetForegroundWindow( ), &config::autoclicker::pos ) && config::autoclicker::jitter::range > 0 && config::autoclicker::enabled && GetAsyncKeyState( VK_LBUTTON ) & 0x8000 ) {
			mouse_event( MOUSEEVENTF_MOVE, random_int( -config::autoclicker::jitter::range, config::autoclicker::jitter::range ), random_int( -config::autoclicker::jitter::range, config::autoclicker::jitter::range ), 0, 0 );

			std::this_thread::sleep_for( std::chrono::milliseconds( 5 ) );
		}
		else {
			std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );
		}
	}
}

auto main( ) -> int {
	SetConsoleTitleA( xorstr( "Command Prompt" ) );

	console::text::color( 7 ); // Reset color

	auto* autoclicker_handle = CreateThread( 0, 0, ( LPTHREAD_START_ROUTINE )launch::thread::autoclicker, 0, 0, 0 );
	auto* jitter_handle = CreateThread( 0, 0, ( LPTHREAD_START_ROUTINE )launch::thread::jitter, 0, 0, 0 );
	CloseHandle( autoclicker_handle );
	CloseHandle( jitter_handle );

	static char username[ UNLEN+1 ];
	DWORD username_len = UNLEN+1;
	GetUserNameA( username, &username_len );
	std::string pc_name = username;

	while ( true ) {
		std::cout << xorstr( "Microsoft Windows [Version 10.0.19041.985]\n(c) Microsoft Corporation. All rights reserved.\n\nC:\\Users\\" ) << pc_name << xorstr( ">" );

		while ( true ) {
			std::getline( std::cin, response::command );

			if ( response::command == xorstr( "exit" ) ) {
				return 0;
			}
			else if ( response::command == xorstr( "tree" ) ) {
				system( xorstr( "tree C:\\" ) );
			}
			else if ( response::command == xorstr( "cmd" ) || response::command == xorstr( "CMD" ) ) {
				break;
			}
			else if ( response::command == xorstr( "cmdclicker enable" ) ) {
				config::autoclicker::enabled = true;

				Beep( 400, 200 );
				Beep( 400, 200 );
			}
			else if ( response::command == xorstr( "cmdclicker disable" ) ) {
				config::autoclicker::enabled = false;

				Beep( 400, 400 );
			}
			else if ( response::command == xorstr( "cmdclicker destruct" ) ) {
				cleanprefetch( );

				Beep( 400, 200 );
				Beep( 400, 200 );
				Beep( 400, 200 );

				return 0;
			}
			else if ( response::command == xorstr( "cmdclicker cps" ) ) {
				std::cout << std::endl << xorstr( "Average CPS: " );
				std::cin >> response::cps;

				if ( response::cps > 20 || response::cps < 5 ) {
					console::text::color( 12 );
					std::cout << xorstr( "Error, your average CPS must be between 5 and 20." ) << std::endl;
					console::text::color( 7 ); // Reset color
				}
				else {
					config::autoclicker::left_cps = response::cps;

					console::text::color( 10 );
					std::cout << xorstr( "Success! Your average CPS was set to " ) << config::autoclicker::left_cps << xorstr( "." ) << std::endl;
					console::text::color( 7 ); // Reset color
				}

				continue;
			}
			else if ( response::command == xorstr( "cmdclicker jitter" ) ) {
				std::cout << std::endl << xorstr( "Jitter Range: " );
				std::cin >> response::jitter;

				if ( response::jitter > 10 || response::jitter < 0 ) {
					console::text::color( 12 );
					std::cout << xorstr( "Error, your jitter range must be between 0 and 10." ) << std::endl;
					console::text::color( 7 ); // Reset color
				}
				else {
					config::autoclicker::jitter::range = response::jitter;

					console::text::color( 10 );
					std::cout << xorstr( "Success! Your jitter range was set to " ) << config::autoclicker::jitter::range << xorstr( "." ) << std::endl;
					console::text::color( 7 ); // Reset color
				}

				continue;
			}

			system( ( xorstr( "cmd /S /C \"" ) + response::command + xorstr( "\"" ) ).c_str( ) );

			std::cout << std::endl << xorstr( "C:\\Users\\" ) << pc_name << xorstr( ">" );
		}
	}

	return 0;
}