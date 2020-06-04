Program  prog;

type
  Test2 = class
	uuu: integer;
  end;
  
  Test1 = class
	ttt: Test2;
  end;

  Test = record
	private
		c: char;
	
	public
		a, b: integer;
		lll: Test1;
		
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
	t, k: Test;
	a, b: integer;
	c, d: boolean;
begin	
	a:= t.Foo('b')* (2 + d);	
end.