@echo on 
echo ɾ��vc�����������ʱ�ļ�
echo ɾ����ʼ...
for /r . %%c in (.) do @if exist "%%c\Debug" rd /S/Q "%%c\Debug"
for /r . %%c in (.) do @if exist "%%c\Release" rd /S/Q "%%c\Release"
for /r . %%c in (*.bsc *.aps *.clw *.ncb *.plg *.positions *.WW *.user *.opensdf *.sdf *.pdb *.ilk *.exp *.orig) do del "%%c"
if exist "ipch" rd /S/Q "ipch"
del /a:h *.suo
echo ɾ�����...
pause