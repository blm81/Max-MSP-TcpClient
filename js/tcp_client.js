inlets = 1;
outlets = 1;

function anything()
{
	var args = arrayfromargs(messagename, arguments);
	parse_obj( JSON.parse( args ) );
}

function bang()
{
	return;
}

//parse and route incoming objects
function parse_obj( obj )
{
	//post( obj.type, obj.data );
	switch( obj.type ) {
		case 'request_ident':
			request_ident();
			break;
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
	ret_val.data = "max";
	send_obj( ret_val );
}