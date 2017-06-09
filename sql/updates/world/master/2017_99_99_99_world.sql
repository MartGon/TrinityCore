-- Fastrider
DELETE FROM `spell_script_names` WHERE `ScriptName`='spell_hun_farstrider';
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) 
VALUES(199564,'spell_hun_farstrider');

-- Fastrider
DELETE FROM `spell_proc_event` WHERE `entry`=199523;
INSERT INTO `spell_proc_event` (`entry`,`SchoolMask`,`SpellFamilyName`,`SpellFamilyMask0`,`SpellFamilyMask1`,`SpellFamilyMask2`,`spellFamilyMask3`,`procFlags`,`procEx`,`ppmRate`,`CustomChance`,`Cooldown`) VALUES
(199523, 1, 9, 0, 0, 0, 0, 0, 2, 0, 0, 0);