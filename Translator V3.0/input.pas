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

var
	t: Test;
	a, b: integer;
	c, d: boolean;
begin	
	a:= b + 2 * a;
end.