Program  prog;

type
  Test = record
	private
		c: char;
	
	public
		a: integer;
		
	function Foo(): char;
	begin
	end;
	
	function Foo(m: char): char;
	var	
		nnn: integer;
	begin
	end;
  end;
  
  Test1 = class	
	a: Test;
	s: char;
	
	private
		function Foo(v: double): char;
		var	
			nnn: integer;
		begin
		end;
  end;

function Foo(m: integer): char;
var	
	nnn: integer;
begin
	m := 1 + nnn;
end;

procedure Proc(g: char);
var	
a: integer;
begin
a:= a + 1;
end;

function Foo(m: char): char;
var	
	nnn: integer;
begin
	m := 1 + nnn;
end;

var
	b, c: boolean; 
	d, k: integer;
begin	
	while(c) do
	begin
		while(c) do
			d:= d + 1;
			
		k:= d;
	end;
	
	k:= 4;
end.