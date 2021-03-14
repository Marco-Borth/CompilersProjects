set breakpoint pending on
set confirm off
file ./cronac
break cronac::Err::report
commands
	where
end
break cronac::InternalError::InternalError
commands
	where
end
break cronac::ToDoError::ToDoError
commands
	where
end

define p4
  set args p4_tests/$arg0.cronac -n --
  run
end
