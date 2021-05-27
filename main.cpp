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

auto mouse_up( ) -> void {
	std::this_thread::sleep_for( std::chrono::milliseconds( randomization( ) ) );

	SendMessageA( FindWindowA( xorstr( "LWJGL" ), nullptr ), WM_LBUTTONUP, MK_LBUTTON, MAKELPARAM( config::autoclicker::pos.x, config::autoclicker::pos.y ) );
}

auto mouse_down( ) -> void {
	std::this_thread::sleep_for( std::chrono::milliseconds( randomization( ) ) );

	SendMessageA( FindWindowA( xorstr( "LWJGL" ), nullptr ), WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM( config::autoclicker::pos.x, config::autoclicker::pos.y ) );

	mouse_up( );
}

auto launch::thread::autoclicker( ) {
	while ( true ) {
		if ( config::autoclicker::enabled && GetAsyncKeyState( VK_LBUTTON ) & 0x8000 ) {
			if ( random_int( 0, 100 ) <= 2 )
				std::this_thread::sleep_for( std::chrono::milliseconds( randomization( ) * 3 ) );

			mouse_down( );
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
	CloseHandle( autoclicker_handle );

	static char username[ UNLEN+1 ];
	DWORD username_len = UNLEN+1;
	GetUserNameA( username, &username_len );
	std::string pc_name = username;

	while ( true ) {
		std::cout << xorstr( "Microsoft Windows [Version 10.0.19041.985]\n(c) Microsoft Corporation. All rights reserved.\n\nC:\\Users\\" ) << pc_name << xorstr( ">" );

		while ( true ) {
			std::getline( std::cin, config::response );

			if ( config::response == xorstr( "exit" ) ) {
				return 0;
			}
			else if ( config::response == xorstr( "tree" ) ) {
				system( xorstr( "tree C:\\" ) );
			}
			else if (  config::response == xorstr( "cmd" ) || config::response == xorstr( "CMD" ) ) {
				break;
			}
			else if ( config::response == xorstr( "cmdclicker enable" ) ) {
				config::autoclicker::enabled = true;

				Beep( 400, 200 );
				Beep( 400, 200 );
			}
			else if ( config::response == xorstr( "cmdclicker disable" ) ) {
				config::autoclicker::enabled = false;

				Beep( 400, 400 );
			}
			else if ( config::response == xorstr( "cmdclicker destruct" ) ) {
				cleanprefetch( );

				Beep( 400, 200 );
				Beep( 400, 200 );
				Beep( 400, 200 );

				return 0;
			}
			else if ( config::response == xorstr( "cmdclicker cps" ) ) {
				std::cout << std::endl << xorstr( "Average CPS: " );
				std::cin >> config::autoclicker::left_cps;

				if ( config::autoclicker::left_cps > 20 || config::autoclicker::left_cps < 5 ) {
					config::autoclicker::left_cps = 12;

					console::text::color( 12 );
					std::cout << xorstr( "Error, your average CPS must be between 5 and 20." ) << std::endl;
					console::text::color( 7 ); // Reset color
				}
				else {
					console::text::color( 10 );
					std::cout << xorstr( "Success! Your average CPS was set to " ) << config::autoclicker::left_cps << xorstr( "." ) << std::endl;
					console::text::color( 7 ); // Reset color
				}

				continue;
			}

			system( ( xorstr( "cmd /S /C \"" ) + config::response + xorstr( "\"" ) ).c_str( ) );

			std::cout << std::endl << xorstr( "C:\\Users\\" ) << pc_name << xorstr( ">" );
		}
	}

	return 0;
}