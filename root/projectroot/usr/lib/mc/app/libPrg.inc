<?// Copyright (c) 2016 Artur Wiebe <artur@4wiebe.de>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
// associated documentation files (the "Software"), to deal in the Software without restriction,
// including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so,
// subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
?>


program
	call log_message(log_prioDebug, "<?=$lib['name']?> prg started")
	call com_putString("/prg", "<?=$lib['name']?>")
	call com_putString("/lib", "<?=$lib['name']?>")
	call log_send
	
	onError
		catch else
			call <?=$lib['name']?>_stopTrigger
			call logErrorFunction("onError")
	end onError

<?if ($lib['prg']['event']) {?>
	onevent <?=$lib['name']?>_prgEvent scantime=<?=$lib['prg']['event']?> 
		try
			call <?=$lib['name']?>_prgEvent
		catch else
			call <?=$lib['name']?>_stopTrigger
			eventoff <?=$lib['name']?>_prgEvent
			call logErrorFunction("onEvent")
		end try
	end onevent
<?}?>

	try
<?if ($lib['prg']['start']) {?>
		call <?=$lib['name']?>_prgStart
<?}?>
<?if ($lib['prg']['event']) {?>
		eventon <?=$lib['name']?>_prgEvent
<?}?>
<?if ($lib['prg']['loop']) {?>
		while <?=$lib['name']?>_prgActive
			call <?=$lib['name']?>_prg
		end while
<?} else {?>
		call <?=$lib['name']?>_prg
<?}?>
	catch else
		call logErrorFunction("prg")
	end try

	call <?=$lib['name']?>_stopTrigger

<?if ($lib['prg']['event']) {?>
	eventoff <?=$lib['name']?>_prgEvent
<?}?>

<?if ($lib['prg']['stop']) {?>
	try
		call <?=$lib['name']?>_prgStop
	catch else
		call logErrorFunction("prgStop")
	end try
<?}?>
	
	call log_message(log_prioDebug, "<?=$lib['name']?> prg stopped")
	call com_putString("/lib", "<?=$lib['name']?>")
	call log_send
end program



sub logErrorFunction(byval func_ as string)	
	call log_message(log_prioError, <?=$lib['prg']['file']?>.error)
	call com_putLong("/error", <?=$lib['prg']['file']?>.errornumber)
	call log_putCodeFunc(func_)
	call com_putString("/prg", "<?=$lib['name']?>")
	call com_putString("/lib", "<?=$lib['name']?>")
	call log_send
end sub
