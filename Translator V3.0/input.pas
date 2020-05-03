type
  Test = record
	private
		c: char;
	
	public
		a: integer;
		
		function Foo(m: integer): char;
		var	
			nnn: integer;
		begin
			m:= 1 + nnn;
		end;
  end;
  
  Test1 = class	
	a: Test;
	s: char;
  end;

 var
	test: Test1; 
	b: integer;
begin	
	b:= test.a.Foo(b) - 5 +99.8;
end.