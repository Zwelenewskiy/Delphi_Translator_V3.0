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
begin	
	t.m := 2;
end.