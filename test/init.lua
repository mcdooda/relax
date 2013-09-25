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

--[[
for k, e in pairs(element.getByTagName('%string')) do
	print(e:getText())
end
]]

element.style {
	relax = {
		['padding'] = '100 100 100 100'
	},
	title = {
		['size-y'] = 20,
		['color']  = '128 128 128 255',
		['margin'] = '0 0 0 0'
	},
	lorem = {
		['size-y']     = 200,
		['padding']    = '10 10 10 10',
		['margin']     = '5 5 5 5',
		['color']      = '255 0 255 255',
	},
	slider = {
		['size']         = '200 20',
		['anchor-y']     = 'top',
		['color']        = '0 0 255 255',
		['onmouseout']   = 'clearSlider(self)',
		['onmousemove']  = 'slider(self)',
		['onmousedown']  = 'printSlider(self)',
		['background']   = 'test/background2.png repeat',
		['margin-top']   = 5
	},
	block = {
		['color']   = '200 80 100 255',
		['padding'] = '20 20 20 20',
		['anchor']  = 'center center'
	}
}

--[[
local sliders = element.getByTagName('slider')
sliders[1]:setAttribute('anchor-x', 'left')
sliders[3]:setAttribute('anchor-x', 'right')
]]

