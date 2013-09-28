-- R is relax' very light selector library inspired by the almighty jQuery

-- library

local R = {}

-- class ElementSet

local ElementSet = {}
ElementSet.__index = ElementSet

-- R(selector)

local function getAtomTag(atom)
	return atom:match('^%a+')
end


local function isAtom(e, atom)
	local atomTag = getAtomTag(atom)
	return e:getTag() == atomTag
end


local function splitSelectors(selector)
	local selectors = {}
	local i = 1
	for s in selector:gmatch('[^,]+') do
		selectors[i] = s
		i = i + 1
	end
	return selectors
end


local function splitAtoms(selector)
	local atoms = {}
	local i = 1
	for atom in selector:gmatch('%S+') do
		atoms[i] = atom
		i = i + 1
	end
	return atoms
end


local function sameBranch(e, root)
	while e and e ~= root do
		e = e:getParent()
	end
	return e == root
end


local function matches(root, e, atoms, index)
	local atom = atoms[index]
	if not e then
		return false
	elseif isAtom(e, atom) then
		if index == 1 then
			if not root then
				return true
			else
				return sameBranch(e, root)
			end
		else
			return matches(root, e:getParent(), atoms, index - 1)
		end
	else
		if root == e then
			return false
		else
			return matches(root, e:getParent(), atoms, index)
		end
	end
end


function find(root, selector)
	local result = ElementSet:new()
	local selectors = splitSelectors(selector)
	local numSelectors = #selectors
	local tags = {}
	for i = 1, numSelectors do
		local selector = selectors[i]
		local atoms = splitAtoms(selector)
		local lastTag = getAtomTag(atoms[#atoms])
		tags[lastTag] = true
	end
	for tag in pairs(tags) do
		local elements = element.getByTagName(tag)
		for i = 1, #elements do
			local e = elements[i]
			for j = 1, numSelectors do
				local selector = selectors[j]
				local atoms = splitAtoms(selector)
				if matches(root, e, atoms, #atoms) then
					result:add(e)
				end
			end
		end
	end
	return result
end


setmetatable(R, {
	
	__call = function(R, ...)
		local result = ElementSet:new()
		local args = {...}
		for i = 1, #args do
			local arg = args[i]
			if type(arg) == 'string' then
				-- selector
				local selector = arg
				result = result + find(nil, selector)
			elseif getmetatable(arg) == ElementSet then
				-- set
				result = result + arg
			else
				-- element
				result:add(arg)
			end
		end
		
		return result
	end
	
})

-- ElementSet methods

function ElementSet:new(...)
	local set = ...
	local o = setmetatable({}, self)
	if getmetatable(set) == self then
		-- 1st argument is already a set
		for e in pairs(set) do
			o:add(e)
		end
	else
		-- elements
		local args = {...}
		for i = 1, #args do
			o:add(args[i])
		end
	end
	return o
end


function ElementSet:__len()
	local len = 0
	for _ in pairs(self) do
		len = len + 1
	end
	return len
end


function ElementSet:__add(set)
	local result = self:clone()
	set:each(function(e)
		result:add(e)
	end)
	return result
end


function ElementSet:__sub(set)
	local result = self:clone()
	set:each(function(e)
		result:remove(e)
	end)
	return result
end


local function elementToString(e)
	local parent = e:getParent()
	if parent then
		return elementToString(parent) .. ' > ' .. e:getTag()
	else
		return e:getTag()
	end
end

function ElementSet:__tostring()
	local output = {}
	output[#output + 1] = #self .. ' elements ===='
	self:each(function(e)
		output[#output + 1] = elementToString(e, true) .. '\t(' .. tostring(e) .. ')'
	end)
	output[#output + 1] = '============'
	return table.concat(output, '\n')
end


function ElementSet:clone()
	return getmetatable(self):new(self)
end


function ElementSet:each(func)
	for e in pairs(self) do
		func(e)
	end
end


function ElementSet:filter(func)
	local result = self:clone()
	self:each(function(e)
		if not func(e) then
			result:remove(e)
		end
	end)
	return result
end


function ElementSet:attr(name, value)
	self:each(function(e)
		e:setAttribute(name, value)
	end)
end


function ElementSet:add(e)
	self[e] = true
end


function ElementSet:remove(e)
	self[e] = nil
end


function ElementSet:find(selector)
	local result = getmetatable(self):new()
	self:each(function(e)
		result = result + find(e, selector)
	end)
	return result
end


return R
