local function getslidervalue(e)
	local x, y = e:getposition()
	local sx, sy = e:getsize()
	local mx, my = mouse.getposition()
	return ((mx - x) / sx)
end

function slider(e)
	local value = getslidervalue(e)
	e:setattribute('color', math.floor((1 - value) * 255)..' '..math.floor(value * 255)..' 0 255')
	local sx, sy = e:getsize()
	e:setattribute('padding-left', value * sx)
end

function printslider(e)
	print(e)
	print(getslidervalue(e))
end

function clearslider(e)
	e:setattribute('color', '0 0 255 255')
end

local y = 0
for k, e in pairs(element.getbytag('slider')) do
	e:setattribute('position-y', y)
	y = y + 30
end

element.style {
	slider = {
		['size-y']       = 20,
		['anchor']       = 'center top',
		['color']        = '0 0 255 255',
		['onmouseout']   = 'clearslider(self)',
		['onmousemove']  = 'slider(self)',
		['onmousedown']  = 'printslider(self)',
		['padding-left'] = 30,
		['background']   = 'test/background.png repeat'
	},
	block = {
		['color']   = '100 100 100 255',
		['padding'] = '20 20 20 20',
		['size']    = '200 '..(y + 30),
		['anchor']  = 'center center'
	}
}

