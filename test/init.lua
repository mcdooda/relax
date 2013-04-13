local function getslidervalue(e)
	local x, y = e:getposition()
	local sx, sy = e:getsize()
	local mx, my = mouse.getposition()
	return ((mx - x) / sx)
end

function slider(e)
	local value = getslidervalue(e)
	e:setattribute('color', math.floor((1 - value) * 255)..' '..math.floor(value * 255)..' 0 255')
end

function printslider(e)
	print(e)
	print(getslidervalue(e))
end

function clearslider(e)
	e:setattribute('color', '0 0 255 255')
end
