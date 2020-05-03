type
  Test = class
	c: char;
  end;

  Test1 = class	
	a, c: Test;
  end; 

 var
	test: Test1; 
	b: integer;
begin
	b:= test.a.c;
end.