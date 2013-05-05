local function getSliderValue(e)
	local x, y = e:getPosition()
	local sx, sy = e:getSize()
	local mx, my = mouse.getPosition()
	return ((mx - x) / sx)
end

function slider(e)
	local value = getSliderValue(e)
	e:setAttribute('color', math.floor((1 - value) * 255)..' '..math.floor(value * 255)..' 0 255')
	local sx, sy = e:getSize()
	e:setAttribute('padding-left', value * sx)
end

function printSlider(e)
	print(e)
	print(getSliderValue(e))
end

function clearSlider(e)
	e:setAttribute('color', '0 0 255 255')
end

local y = 240
for k, e in pairs(element.getByTagName('slider')) do
	e:setAttribute('position-y', y)
	y = y + 30
end

element.style {
	relax = {
		['padding'] = '100 100 100 100'
	},
	title = {
		['size-y'] = 20
	},
	lorem = {
		['size-y']     = 200,
		['position-y'] = 30,
		['padding']    = '10 10 10 10'
	},
	slider = {
		['size-y']       = 20,
		['anchor']       = 'center top',
		['color']        = '0 0 255 255',
		['onmouseout']   = 'clearSlider(self)',
		['onmousemove']  = 'slider(self)',
		['onmousedown']  = 'printSlider(self)',
		['padding-left'] = 30,
		['background']   = 'test/background2.png repeat'
	},
	block = {
		['color']   = '200 80 100 255',
		['padding'] = '20 20 20 20',
		['size-y']  = y + 30,
		['anchor']  = 'center center'
	}
}

