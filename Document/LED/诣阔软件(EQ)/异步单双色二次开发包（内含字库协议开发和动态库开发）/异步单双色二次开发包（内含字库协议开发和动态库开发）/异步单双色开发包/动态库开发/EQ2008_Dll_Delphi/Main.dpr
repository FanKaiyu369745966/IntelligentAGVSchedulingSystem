program Main;

uses
  Forms,
  EQ2008DLL in 'EQ2008DLL.pas' {Form1},
  EQ2008_Function in 'EQ2008_Function.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.CreateForm(TForm1, Form1);
  Application.Run;
end.
