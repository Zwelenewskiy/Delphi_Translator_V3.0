type
  Test = class
	private
		c: char;
	
	public
		a: integer;
  end;
  
  Test1 = class	
	a: Test;
  end;

 var
	test: Test1; 
	b: integer;
begin
	test.a.a:= b;
end.