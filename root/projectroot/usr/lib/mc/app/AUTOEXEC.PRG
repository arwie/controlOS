program continue

	call state_init
	call log_init
	call logSystemStart
	call state_start
	call log_start


	onSystemError
		catch else
			call log_message(log_prioError, system.error)
			call com_putLong("error", system.errornumber)
			call log_putCodeFunc("onSystemError")
			call log_send
	end onSystemError


	call system_init
	call system_start

	while system_isRunning and system.enable
		sleep 500
	end while


	call logSystemStop
	sleep 1000	'let all log messages go out
'	call log_stop
'	call state_stop


'	call com_closeAll
'	print sh("reboot")
	
terminate program



sub logSystemStart
	call log_message(log_prioInfo, "mc system start")
	call com_putString("version", version)
	call com_putString("sn",  system.serialNumber)
	call com_putString("uac", system.userAuthorizationCode)
	call log_putMessageId("94cb9b9f452e4309a1265c857c5321a2")
	call log_send
end sub


sub logSystemStop
	call log_message(log_prioNotice, "mc system stop")
	call log_putMessageId("e0dc5bc5c2914fab953d61df01c9d5eb")
	call log_send
end sub
