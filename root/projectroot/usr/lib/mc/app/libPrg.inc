
program
	call log_message(log_prioDebug, "<?=$lib['name']?> prg started")
	call com_putString("prg", "<?=$lib['name']?>")
	call com_putString("lib", "<?=$lib['name']?>")
	call log_send
	
	onError
		catch else
			<?=$lib['name']?>_prgActive = false
			call logErrorFunction("onError")
	end onError

<?if ($lib['prg']['event']) {?>
	onevent <?=$lib['name']?>_prgEvent scantime=<?=$lib['prg']['event']?> 
		try
			call <?=$lib['name']?>_prgEvent
		catch else
			<?=$lib['name']?>_prgActive = false
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

	<?=$lib['name']?>_prgActive = false

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
	call com_putString("lib", "<?=$lib['name']?>")
	call log_send
end program



sub logErrorFunction(byval func_ as string)	
	call log_message(log_prioError, <?=$lib['prg']['file']?>.error)
	call com_putLong("error", <?=$lib['prg']['file']?>.errornumber)
	call log_putCodeFunc(func_)
	call com_putString("prg", "<?=$lib['name']?>")
	call com_putString("lib", "<?=$lib['name']?>")
	call log_send
end sub
