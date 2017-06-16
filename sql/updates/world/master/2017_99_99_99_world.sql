-- Hunter Spell Scripts

-- Spell Scripts
DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_hun_flanking_strike','spell_hun_flanking_strike_pet','spell_hun_mongoose_bite','spell_hun_aspect_eagle','spell_hun_hunting_companion_gain_charge','spell_hun_hunting_companion');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(202800,'spell_hun_flanking_strike'),               -- Flanking Strike (202800)
(204740,'spell_hun_flanking_strike_pet'),           -- Flanking Strike pet version (204740)
(190928,'spell_hun_mongoose_bite'),                 -- Mongoose Bite (190928)
(186289,'spell_hun_aspect_eagle'),                  -- Aspect of the Eagle (186289)
(191335,'spell_hun_hunting_companion_gain_charge'), -- Hunting Companion gain charge (191335)
(191336,'spell_hun_hunting_companion');             -- Hunting Companion pet aura (191336)


DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_hun_arcane_shot','spell_hun_multi_shot');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(185358,'spell_hun_arcane_shot'), -- Arcane shot
(2643,'spell_hun_multi_shot');    -- Multi shot

-- 194278 Caltrops
DELETE FROM `areatrigger_template` WHERE `Id`=3157;
INSERT INTO `areatrigger_template` (`Id`, `Type`, `Flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES
(3157, 0, 0, 5, 5, 0, 0, 0, 0, "areatrigger_hun_caltrops", 23911);

DELETE FROM `spell_areatrigger` WHERE `SpellMiscId`=3157;
INSERT INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES
(5084, 3157, 0, 0, 0, 0, 0, 0, 15000, 23911);

-- 199483 Camouflage
DELETE FROM `spell_script_names` WHERE `entry`=199483;
INSERT INTO `spell_script_names`(spell_id,ScriptName) VALUES(199483,"spell_hun_camouflage");

-- 1543 Flare areatrigger_template and areatrigger script
DELETE FROM `areatrigger_template` WHERE Id=3154;
INSERT INTO `areatrigger_template` (`Id`, `Type`, `Flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES
(3154, 0, 0, 10, 10, 0, 0, 0, 0, "areatrigger_hun_flare", 23911);

DELETE FROM `spell_areatrigger` WHERE SpellMiscId=3154
INSERT INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES
(510, 3154, 0, 0, 0, 0, 0, 0, 2000, 23911);

-- 13813 Explosive Trap
DELETE FROM `areatrigger_template` WHERE `Id`=3158;
INSERT INTO `areatrigger_template` (`Id`, `Type`, `Flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES
(3158, 0, 0, 5, 5, 0, 0, 0, 0, "areatrigger_hun_explosive_trap", 23911);

DELETE FROM `spell_areatrigger` WHERE `SpellMiscId`=3158;
INSERT INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES
(1613, 3158, 0, 0, 0, 0, 0, 0, 60000, 23911);

-- 190925 Harpoon
DELETE FROM spell_script_names WHERE spell_id=190925;
INSERT INTO spell_script_names(spell_id, ScriptName) VALUES(190925,"spell_hun_harpoon");

-- 3355 Ice trap
DELETE FROM areatrigger_template WHERE Id=3155;
INSERT INTO `areatrigger_template` (`Id`, `Type`, `Flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES
(3155, 0, 0, 3, 3, 0, 0, 0, 0, "areatrigger_hun_freezing_trap", 23911);

DELETE FROM spell_areatrigger WHERE SpellMiscId=4424;
INSERT INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES
(4424, 3155, 0, 0, 0, 0, 0, 0, 6000, 23911);

-- 201091 Mortal Wounds
DELETE FROM `spell_script_names` WHERE `entry`=201091;
INSERT INTO `spell_script_names`(spell_id,ScriptName) VALUES(201091,"spell_hun_mortal_wounds");

-- 206505 Murder of Crows (Survival)
DELETE FROM `spell_script_names` WHERE `entry`=206505;
INSERT INTO `spell_script_names`(spell_id,ScriptName) VALUES(206505,"spell_hun_murder_crows");

-- 87935 Serpent Sting
DELETE FROM `spell_script_names` WHERE `ScriptName` IN ('spell_hun_carve','spell_hun_raptor_strike');
INSERT INTO `spell_script_names` (`spell_id`, `ScriptName`) VALUES
(187708,'spell_hun_carve'), 	      -- 187708 Carve
(186270,'spell_hun_raptor_strike');   -- 186270 Raptor strike

-- 201078 Snake Hunter
DELETE FROM `spell_script_names` WHERE `entry`=201078;
INSERT INTO `spell_script_names`(spell_id,ScriptName) VALUES(201078,"spell_hun_snake_hunter");

-- 191241 Sticky Bomb
DELETE FROM `spell_script_names` WHERE `entry`=191241;
INSERT INTO `spell_script_names`(spell_id,ScriptName) VALUES(191241,"spell_hun_sticky_bomb");

-- 187699 Tar trap starter areatrigger
DELETE FROM `areatrigger_template` WHERE `Id`=3159;
INSERT INTO `areatrigger_template` (`Id`, `Type`, `Flags`, `Data0`, `Data1`, `Data2`, `Data3`, `Data4`, `Data5`, `ScriptName`, `VerifiedBuild`) VALUES
(3159, 0, 0, 5, 5, 0, 0, 0, 0, "areatrigger_hun_tar_trap", 23911);

DELETE FROM `spell_areatrigger` WHERE `SpellMiscId`=3159;
INSERT INTO `spell_areatrigger` (`SpellMiscId`, `AreaTriggerId`, `MoveCurveId`, `ScaleCurveId`, `MorphCurveId`, `FacingCurveId`, `DecalPropertiesId`, `TimeToTarget`, `TimeToTargetScale`, `VerifiedBuild`) VALUES
(4435, 3159, 0, 0, 0, 0, 0, 0, 60000, 23911);