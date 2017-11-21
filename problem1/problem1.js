//Node application
//Note: I had the idea to store users and groups in a database since there would be no point
//to having users and groups if the information could not be stored in any way. I decided
//to use firebase as my database which requires the installing of the firebase npm package.
//However, since non-standard libraries are not allowed, I have commented out the code that would
//be my implementation for the database.

// const firebase = require("firebase");
//Core module
const readline = require('readline');

/*
class firebaseDB {
  //Initialize firebase
  constructor() {
    const config = {
      apiKey: {key},
      authDomain: {domain},
      databaseURL: {url},
      projectId: {id},
      storageBucket: {storage bucket},
      messagingSenderId: {id}
    };
    firebase.initializeApp(config);
    this.database = firebase.database();    
  }

  //Check if group exists
  //Returns a promise
  validateGroup(group) {
    return firebase.database().ref('/groups').once('value').then((snapshot) => {
      if (snapshot.val()[group]) return true;
      else                       return false;
    });
  }

  //Push user to group in database and disconnect from database
  pushAndGoOffline(group, user) {
    firebase.database().ref('groups/' + group).push({
      username: user
    }).then(() => {
      //Close database once all writes to database have finished
      this.goOffline();
    });
  }

  //Push user to group in database
  pushUser(group, user) {
    firebase.database().ref('groups/' + group).push({
      username: user
    });
  }

  //Disconnect from database
  goOffline() {
    firebase.database().goOffline();    
  }

  //Returns a promise which can be used to retrieve a sorted list of users in the group
  //Example usage:
  //fireDB.getUsersInGroup('admins').then((data) => {
  //  console.log(data);
  //});
  //Example output: [ 'alan', 'david' ]
  getUsersInGroup(group) {
    this.validateGroup(group).then((exists) => {
      if (exists) {
        return firebase.database().ref('/groups/' + group).once('value').then((snapshot) => {
          const userObj = snapshot.val();
          let users = [];
          Object.keys(userObj).forEach((id) => {
            users.push(userObj[id].username);
          });
          return users.sort();
        });
      }
    });
  }

  //Returns a promise which can be used to retrieve a sorted list of groups
  getGroups() {
    return firebase.database().ref('groups/').once('value').then((snapshot) => {
      const groups = snapshot.val();
      let line = [];
      Object.keys(groups).forEach((group) => {
        line.push(group);
      });
      return line.sort();
    });
  }

  //Gives a reference of the location of the user in the database
  //Returns a promise
  findUserReference(user) {
    return firebase.database().ref('/groups').once('value').then((snapshot) => {
      const groups = snapshot.val();
      const groupArr = Object.keys(groups);

      for (let i = 0; i < groupArr.length; i++) {
        const groupObj = groups[groupArr[i]];
        const idArr = Object.keys(groupObj);
        for (let j = 0; j < idArr.length; j++) {
          if (groupObj[idArr[j]].username === user) {
            return '/groups/' + groupArr[i] + '/' + idArr[j];
          }
        }
      }
    });
  }

  //Delete user (case sensitive)
  deleteUser(user) {
    this.findUserReference(user).then((ref) => {
      if (ref) {
        firebase.database().ref(ref).remove();
      }
    });
  }

  //Delete group
  deleteGroup(group) {
    this.validateGroup(group).then((exists) => {
      if (exists) {
        firebase.database().ref('/groups/' + group).remove();        
      }
    });
  }
} //End class
*/

// const fireDB = new firebaseDB();  

//Print the users in one specific group
// function printGroup(group) {
//   let line = group + ',';
//   fireDB.getUsersInGroup(group).then((users) => {
//     users.forEach((user, index) => {
//       if (index === users.length - 1) line += user;
//       else                            line += user + ',';
//     });
//     console.log(line);
//   });
// }

//Print groups and users based on given input and store groups and users to database
// function printAndStore(userGroup) {
//   //If input was empty, do nothing and disconnect from database to end execution
//   if (Object.keys(userGroup).length === 0) {
//     // fireDB.goOffline();
//     return;
//   }

//   //Print sorted groups and users and store to database
//   Object.keys(userGroup).sort().forEach((group) => {
//     let line = group + ',';
//     userGroup[group].sort().forEach((user, index) => {
//       // fireDB.pushAndGoOffline(group, user);
//       if (index === userGroup[group].length - 1)  line += user;
//       else                                        line += user + ',';
//     });
//     console.log(line);
//   });
// }

//Create object to read from and write to standard in
const rl = readline.createInterface({
  input: process.stdin,
  output: process.stdout,
  terminal: false,
});

let userGroup = {}  //Stores the groups and users provided by stdin

//Make sure that the input is valid
function validInput(line) {
  const regex = /^[a-zA-Z ]+$/;
  return regex.test(line);
}

//Print groups and users based on given input
function printGroupsAndUsers(userGroup) {
  //If input is empty, do nothing
  if (Object.keys(userGroup).length === 0) {
    return;
  }

  //Print sorted groups and users
  Object.keys(userGroup).sort().forEach((group) => {
    let line = group + ',';
    userGroup[group].sort().forEach((user, index) => {
      if (index === userGroup[group].length - 1)  line += user;
      else                                        line += user + ',';
    });
    console.log(line);
  });
}

//Read each input and store the group and user from each line in userGroup object
rl.on('line', (line) => {
  //Handle invalid input
  if (!validInput(line)) {
    console.error('Invalid input: ' + line);
    process.exit();  
  }

  //Trim any extra space between words and split line
  line = line.replace(/\s+/g, ' ');
  line = line.split(' '); 

  //Make sure that each line has the correct number of arguments
  if (line.length != 2) {
    console.error('Invalid input');
    process.exit();
  }

  //Add information from line to userGroup object
  if (!userGroup[line[1]]) {
    userGroup[line[1]] = [];
    userGroup[line[1]].push(line[0]);
  } else {
    userGroup[line[1]].push(line[0]);    
  }
})

//If all lines from stdin were successfuly read, then print data in userGroup
rl.on('close', () => {
  printGroupsAndUsers(userGroup);  
})
