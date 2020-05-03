type
  Test = record
	private
		c: char;
	
	public
		a: integer;
  end;
  
  Test1 = class	
	a: Test;
	
	procedure Proc(m: integer);
	begin
	end;
  end;

 var
	test: Test1; 
	b: integer;
begin
	b:= test.Proc(b) + 1 - 1321.99667;
end.