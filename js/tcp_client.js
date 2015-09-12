inlets = 2;
outlets = 2;

var name = "max"; //use to route messages to correct live device

function anything()
{
	var args = arrayfromargs(messagename, arguments);
	if ( inlet == 0 ) //from tcp server
		parse_obj( JSON.parse( args ) );
	else if ( inlet == 1 ) //from within patch
		eval_sym( args );
}

function bang()
{
	return;
}

//parse and route incoming object from tcp server
function parse_obj( obj )
{
	switch( obj.type ) {
		case 'request_ident':
			request_ident();
			break;
		case 'from_cinder':
			outlet( 1, obj.id + " " + obj.data );
			break;
	}
}

//evaluate symbol from within max
function eval_sym( args )
{
	var output = {};
	output.type = null;
	switch ( args[0] ) {
		case 'to_server':
			output.type = "to_cinder";
			try {
				output.data = JSON.parse( args[1] );
			}
			catch( exception ) {
				post( "JSON parse exception: ", exception );
			}
		break; //'to_server'

		case 'set_name':
			name = args[1];
		break; //'set_name'
	}
	//filter incorrectly formatted input
	if ( output.type != null ) {
		send_obj( output );
	}
}

//send json back to server
function send_obj( obj )
{
	outlet( 0, JSON.stringify( obj ) );
}

//identify client
function request_ident()
{
	var ret_val = {};
	ret_val.type = "ident";
	ret_val.data = name;
	send_obj( ret_val );
}