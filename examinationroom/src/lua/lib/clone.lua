-- Copy-on-write Clone

-- Limitations:
-- Can not generally clone tables that have metatables (Strips/Replaces MetaTable if possible)
-- Can not clone Userdata or Methods with upvalues (Copies instead, possible leak)
-- Clones are not eqal to the original

clone = {};

-- Creates and returns a deep copy as faithful as possible
-- This works similar as persistence.write() for multi refs
clone.deep = function (t)

end;

-- Creates and returns a copy-on-write table
clone.cow = function (t)

end;
