local hpack = require "hpack"

local pack = hpack.new()

print(pack)

local r = pack:encode("content-length", "2145")
print(pack:decode(r))

local headers = {}
headers["content-lenght"] = "214"
headers["etag"] = "sdjfsxxxxxxxxxx"

for k, v in pairs(headers) do
    local r, l = pack:encode(k, v)
    print(#r, l, pack:decode(r))
end
