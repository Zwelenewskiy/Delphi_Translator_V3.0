procedure Proc();
var
	b: boolean;
begin
	if(b) then
		Proc();
	
	while(b) do
	begin
		b:= 1;
	end;
end;

function Foo(a: integer): boolean;
begin
end;

var
	a: integer;
begin				
	a:= foo();
	
	PROC();
end.