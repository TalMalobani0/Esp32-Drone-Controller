extends Node2D

@onready var joystick_knob: Sprite2D = $Joystick_Knob
var max_range = 100.0  # how far the joystick_knob can move from center
var isPressed = false  # false means not being touched
var output = Vector2.ZERO

func _input(event):
	if event is InputEventMouseButton or event is InputEventScreenTouch:
		if event.pressed:
			if event.position.distance_to(global_position) < max_range:
				isPressed = true
		else:
			isPressed = false
			reset_joystick()

	# Handle Dragging
	if (event is InputEventMouseMotion or event is InputEventScreenDrag) and isPressed != false:
		var center = global_position
		var vector = event.position - center
		
		if name == "RightJoystick":
			if abs(vector.x) > abs(vector.y):
				vector.y = 0 # Lock vertical
			else:
				vector.x = 0 # Lock horizontal
		
		# keep the joystick_knob inside the circle
		if vector.length() > max_range:
			vector = vector.normalized() * max_range
		
		joystick_knob.position = vector
		output = vector / max_range # Returns -1.0 to 1.0

func reset_joystick():
	joystick_knob.position = Vector2.ZERO
	output = Vector2.ZERO

func get_value():
	return output
